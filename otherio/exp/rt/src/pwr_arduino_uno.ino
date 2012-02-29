//
// Proview
// Copyright (C) 2010 SSAB Oxelösund AB.
//
// This program is free software; you can redistribute it and/or 
// modify it under the terms of the GNU General Public License as 
// published by the Free Software Foundation, either version 2 of 
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License 
// along with the program, if not, write to the Free Software 
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// Description:
// Sketch for Arduino USB board to communicate with Proview I/O
// object OtherIO:Arduino_Uno.
//
byte firmware[20] = "Proview 2012-02-29";
byte version_major = 2;
byte version_minor = 0;
byte msg = 0;
byte sts;
int val = 0;
byte amsg[50];
byte smsg[10];
byte diSize = 0;
byte doSize = 0;
byte aiSize = 0;
byte aoSize = 0;
byte diMask[10];
byte doMask[10];
byte aiMask[4];
byte aoMask[4];
int watchdogTime = 5000;
int status;
int i;
int j;
int aiList[32];
int aiCnt;
int aoList[32];
int aoCnt;
byte msgType;
byte msgId;
byte msgSize;
byte msgData[100];
byte rmsg[40];
int sizeErrorCnt = 0;
int noMessageCnt = 0;
const int delayTime = 1;
const int debug = 0;

const int MSG_TYPE_DOWRITE = 1;
const int MSG_TYPE_DIREAD = 2;
const int MSG_TYPE_AIREAD = 3;
const int MSG_TYPE_AOWRITE = 4;
const int MSG_TYPE_CONFIGURE = 5;
const int MSG_TYPE_STATUS = 6;
const int MSG_TYPE_DEBUG = 7;
const int MSG_TYPE_WRITEALL = 8;
const int MSG_TYPE_READALL = 9;
const int MSG_TYPE_CONNECTREQ = 10;
const int MSG_TYPE_CONNECTRES = 11;

const int ARD__SUCCESS = 1;
const int ARD__DICONFIG = 2;
const int ARD__DOCONFIG = 4;
const int ARD__AICONFIG = 6;
const int ARD__AOCONFIG = 8;
const int ARD__COMMERROR = 10;
const int ARD__MSGSIZE = 12;
const int ARD__NOMSG = 14;
const int ARD__CHECKSUM = 16;

void sendDebug( byte sts)
{
  rmsg[0] = 4;
  rmsg[1] = 0;
  rmsg[2] = MSG_TYPE_DEBUG;
  rmsg[3] = sts;
  add_checksum( rmsg);
  Serial.write( rmsg, rmsg[0]);
}

void add_checksum( byte *buf)
{
  int i;
  byte sum = 0;

  buf[0]++;
  for ( i = 0; i < buf[0] - 1; i++)
    sum ^= buf[i];

  buf[buf[0]-1] = sum;
}

int check_checksum( byte size, byte id, byte type, byte *buf)
{
  int i;
  unsigned char sum = 0;

  sum ^= size;
  sum ^= id;
  sum ^= type;
  for ( i = 0; i < size - 4; i++)
    sum ^= buf[i];

  if ( buf[size-1] == sum)
    return 1;
  return 0;
}

//
//  Reset all outputs when communication communication is down
//
void resetOutput()
{  
  for ( i = 0; i < doSize; i++) {
    for ( j = 0; j < 8; j++) {
      if ( ((1 << j) & doMask[i]) != 0)
        digitalWrite( i * 8 + j, LOW);
    }
  }
  for ( i = 0; i < aoCnt; i++)
    analogWrite( aoList[i], 0);
}

//
//  Read a message from the serial port
//
int serialRead()
{
  int num;

  num = Serial.available();
  if ( num == 0)
    return ARD__NOMSG;
    
  msgSize = Serial.peek();
  if ( num < msgSize)
    return ARD__MSGSIZE;
    
  msgSize = Serial.read();

  msgId = Serial.read();
  msgType = Serial.read();
  msgSize -= 3;

  for ( int i = 0; i < msgSize; i++)
    msgData[i] = Serial.read();
    
  if ( !check_checksum( msgSize, msgId, msgType, msgData))
    return ARD__CHECKSUM;

  if ( debug) {
    rmsg[0] = msgSize - 1 + 3;
    rmsg[1] = msgId;
    rmsg[2] = MSG_TYPE_DEBUG;
    for ( int j = 0; j < msgSize-1; j++)
      rmsg[j+3] = msgData[j];
    add_checksum( rmsg);
    Serial.write( rmsg, rmsg[0]);
  }
  
  return ARD__SUCCESS;
}

void setup()
{
  // Start serial port at the configured baud rate
  Serial.begin(9600);
  Serial.flush();
}

void loop()
{
  
  status = serialRead();
  if ( status == ARD__NOMSG) {
    if ( watchdogTime != 0) {
      // Increment watchdog counter
      noMessageCnt++;
       if ( noMessageCnt * delayTime > watchdogTime)
         resetOutput();
    }
  }
  else if ( status == ARD__MSGSIZE) {
    sizeErrorCnt++;
    if ( sizeErrorCnt > 50) {
      Serial.flush();
      sizeErrorCnt = 0;
    }
  }
  else if ( (status & 1) != 0) {
    // A message is received
    sizeErrorCnt = 0;
    noMessageCnt = 0;

    if ( msgType == MSG_TYPE_DOWRITE) {
      // Write digital outputs

      if ( msgSize == doSize) {
        for ( i = 0; i < doSize; i++) {
          for ( j = 0; j < 8; j++) {
            if ( ((1 << j) & doMask[i]) != 0) {
              if ( ((1 << j) & msgData[i]) != 0)
                digitalWrite( i * 8 + j, HIGH);          
              else
                digitalWrite( i * 8 + j, LOW);          
            }
          }
        }
        sts = ARD__SUCCESS;
      }
      else {
        sts = ARD__COMMERROR;
      }
    }
    else if ( msgType == MSG_TYPE_AOWRITE) {
      // Write analog outputs

      if ( msgSize == aoCnt) {
        for ( i = 0; i < aoCnt; i++)
   	  analogWrite( aoList[i], msgData[i]);

        sts = ARD__SUCCESS;
      }
      else {
        sts = ARD__COMMERROR;
      }
    }
    else if ( msgType == MSG_TYPE_WRITEALL) {
      // Write digital outputs

      if ( msgSize == doSize + aoCnt) {
        for ( i = 0; i < doSize; i++) {
          for ( j = 0; j < 8; j++) {
            if ( ((1 << j) & doMask[i]) != 0) {
              if ( ((1 << j) & msgData[i]) != 0)
                digitalWrite( i * 8 + j, HIGH);          
              else
                digitalWrite( i * 8 + j, LOW);          
            }
          }
        }

        for ( i = 0; i < aoCnt; i++)
   	  analogWrite( aoList[i], msgData[doSize + i]);

        sts = ARD__SUCCESS;
      }
      else {
        sts = ARD__COMMERROR;
      }
    }
    else if ( msgType == MSG_TYPE_DIREAD) {
      // Read Digital inputs
      smsg[0] = diSize + 3;
      smsg[1] = msgId;
      smsg[2] = MSG_TYPE_DIREAD;
      for ( i = 0; i < diSize; i++) {
        smsg[i + 3] = 0;
        for ( j = 0; j < 8; j++) {
          if ( ((1 << j) & diMask[i]) != 0) {
            val = digitalRead( i * 8 + j);
            if ( val == HIGH) 
              smsg[i + 3] |= 1 << j;
          }
        }
      }
      add_checksum( smsg);
      Serial.write( smsg, smsg[0]);
    }
    else if ( msgType == MSG_TYPE_AIREAD) {
      // Read analog inputs
      amsg[0] = aiCnt * 2 + 3;
      amsg[1] = msgId;
      amsg[2] = MSG_TYPE_AIREAD;
      for ( i = 0; i < aiCnt; i++) {
        val = analogRead( aiList[i]);
        amsg[i*2 + 3] = val / 256;
        amsg[i*2 + 1 + 3] = val % 256;
      }
      add_checksum( amsg);
      Serial.write( amsg, amsg[0]);
    }
    else if ( msgType == MSG_TYPE_READALL) {
      // Read Digital inputs
      amsg[0] = diSize + aiCnt * 2 + 3;
      amsg[1] = msgId;
      amsg[2] = MSG_TYPE_READALL;
      for ( i = 0; i < diSize; i++) {
        amsg[i + 3] = 0;
        for ( j = 0; j < 8; j++) {
          if ( ((1 << j) & diMask[i]) != 0) {
            val = digitalRead( i * 8 + j);
            if ( val == HIGH) 
              amsg[i + 3] |= 1 << j;
          }
        }
      }

      for ( i = 0; i < aiCnt; i++) {
        val = analogRead( aiList[i]);
        amsg[diSize + i*2 + 3] = val / 256;
        amsg[diSize + i*2 + 1 + 3] = val % 256;
      }
      add_checksum( amsg);
      Serial.write( amsg, amsg[0]);
    }
    else if ( msgType == MSG_TYPE_CONFIGURE) {
      // Configure message
      int offs = 0;
      sts = ARD__SUCCESS;

      if ( debug) {
        smsg[0] = msgSize + 3;
        smsg[1] = msgId;
        smsg[2] = MSG_TYPE_DEBUG;
        for ( int j = 0; j < msgSize; j++)
          smsg[j+3] = msgData[j];
	add_checksum( smsg);
        Serial.write( smsg, smsg[0]);
      }
      
      watchdogTime = msgData[offs++] * 100;
      
      diSize = msgData[offs++];
      if ( diSize > 10) {
        diSize = 10;
        sts = ARD__DICONFIG;
      }
      if ( sts & 1 != 0) {  
        for ( i = 0; i < diSize; i++)
          diMask[i] = msgData[offs++];
      }        

      if ( sts & 1 != 0) {  
        doSize = msgData[offs++];
        if ( doSize > 10) {
          doSize = 10;
          sts = ARD__DOCONFIG;
        }
      }
      if ( sts & 1 != 0) {        
        for ( i = 0; i < doSize; i++)
          doMask[i] = msgData[offs++];
      }

      if ( sts & 1 != 0) {        
        aiSize = msgData[offs++];
        if ( aiSize > 4) {
          aiSize = 4;
          sts = ARD__AICONFIG;
        } 
      }

      if ( sts & 1 != 0) {        
        for ( i = 0; i < aiSize; i++)
          aiMask[i] = msgData[offs++];
      }

      if ( sts & 1 != 0) {        
        aoSize = msgData[offs++];
        if ( aoSize > 4) {
          aoSize = 4;
          sts = ARD__AOCONFIG;
        }        
      }
      
      if ( sts & 1 != 0) {  
        for ( i = 0; i < aoSize; i++)
          aoMask[i] = msgData[offs++];
      }

      if ( sts & 1 != 0) {  
        // Set Di pinmode
        for ( i = 0; i < diSize; i++) {
          for ( j = 0; j < 8; j++) {
            if ( ((1 << j) & diMask[i]) != 0)
              pinMode( i * 8 + j, INPUT);          
          }
        }
        
        // Set Do pinmode
        for ( i = 0; i < doSize; i++) {
          for ( j = 0; j < 8; j++) {
            if ( ((1 << j) & doMask[i]) != 0)
              pinMode( i * 8 + j, OUTPUT);          
          }
        }  

        // Create list of configured Ai
        aiCnt = 0;
        for ( i = 0; i < aiSize; i++) {
          for ( j = 0; j < 8; j++) {
            if ( ((1 << j) & aiMask[i]) != 0) {
              aiList[aiCnt] = i * 8 + j;
              aiCnt++;
            }
          }
        }

        // Create list of configured Ao
        aoCnt = 0;
        for ( i = 0; i < aoSize; i++) {
          for ( j = 0; j < 8; j++) {
            if ( ((1 << j) & aoMask[i]) != 0) {
              aoList[aoCnt] = i * 8 + j;
              aoCnt++;
            }
          }
        }
      }
      
      // Send configuration status
      smsg[0] = 4;
      smsg[1] = msgId;
      smsg[2] = MSG_TYPE_STATUS;
      smsg[3] = sts;
      add_checksum( smsg);
      Serial.write(smsg, smsg[0]);
       
    }
    else if ( msgType == MSG_TYPE_CONNECTREQ) {
      // Connect reqeust
      amsg[0] = 23 + 3;
      amsg[1] = msgId;
      amsg[2] = MSG_TYPE_CONNECTRES;
      amsg[3] = ARD__SUCCESS;
      amsg[4] = version_major;
      amsg[5] = version_minor;
      for ( i = 0; i < 20; i++)
        amsg[i+6] = firmware[i];
      add_checksum( amsg);
      Serial.write( amsg, amsg[0]);
    }
  }
  else {
    // Return error status
    smsg[0] = 4;
    smsg[1] = msgId;
    smsg[2] = MSG_TYPE_STATUS;
    smsg[3] = status;
    add_checksum( smsg);
    Serial.write(smsg, smsg[0]);
    
  }

  delay(delayTime); 
}
