//
// Gertboard test
//
// SPI (ADC/DAC) control code
//
// This code is part of the Gertboard test suite
// These routines access the AD and DA chips
//
//
// Copyright (C) Gert Jan van Loo & Myra VanInwegen 2012
// No rights reserved
// You may treat this program as if it was in the public domain
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//
// Try to strike a balance between keep code simple for
// novice programmers but still have reasonable quality code
//

#include "rt_io_gb_common.h"
#include "rt_io_gb_spi.h"

//
// Set-up the SPI interface
//
// Speed depends on what you talk to
// In this case use 1MHz
//
void setup_spi()
{
  // Want to have 1 MHz SPI clock.
  // Assume 250 Mhz system clock
  // So divide 250MHz system clock by 250 to get 1MHz45
  SPI0_CLKSPEED = 250;

  // clear FIFOs and all status bits
  SPI0_CNTLSTAT = SPI0_CS_CLRALL;
  SPI0_CNTLSTAT = SPI0_CS_DONE; // make sure done bit is cleared
} // setup_spi()

//
// Read a value from one of the two ADC channels
//
// To understand this code you had better read the
// datasheet of the AD chip (MCP3002)
//
int read_adc(int chan) // 'chan' must be 0 or 1. This is not checked!
{ unsigned char v1,v2,rec_c;
  int status,w;
  // Set up for single ended, MS comes out first
  v1 = 0xD0 | (chan<<5);
  // Delay to make sure chip select is high for a short while
  short_wait();

  // Enable SPI interface: Use CS 0 and set activate bit
  SPI0_CNTLSTAT = SPI0_CS_CHIPSEL0|SPI0_CS_ACTIVATE;

  // Write the command into the FIFO so it will
  // be transmitted out of the SPI interface to the ADC
  // We need a 16-bit transfer so we send a command byte
  // folowed by a dummy byte
  SPI0_FIFO = v1;
  SPI0_FIFO = 0; // dummy

  // wait for SPI to be ready
  // This will take about 16 micro seconds
  do {
     status = SPI0_CNTLSTAT;
  } while ((status & SPI0_CS_DONE)==0);
  SPI0_CNTLSTAT = SPI0_CS_DONE; // clear the done bit

  // Data from the ADC chip should now be in the receiver
  // read the received data
  v1 = SPI0_FIFO;
  v2 = SPI0_FIFO;
  // Combine the 8-bit and 2 bit values into an 10-bit integer
  // NOT!!!  return ((v1<<8)|v2)&0x3FF;
  // I have checked the result and it returns 3 bits in the MS byte not 2!!
  // So I might have my SPI clock/data pahse wrong.
  // For now its easier to dadpt the results (running out of time)
  return ( (v1<<7) | (v2>>1) ) & 0x3FF;
} // read_adc

//
// Write 12 bit value to DAC channel 0 or 1
//
// To understand this code you had better read the
// datasheet of the AD chip (MCP4802/MCP4812/MCP4822)
//
void write_dac(int chan, // chan must be 0 or 1, this is not checked
                int val) // chan must be max 12 bit
{ char v1,v2,dummy;
  int status;
  val &= 0xFFF;  // force value in 12 bits

  // Build the first byte: write, channel 0 or 1 bit
  // and the 4 most significant data bits
  v1 = 0x30 | (chan<<7) | (val>>8);
  // Remain the Least Significant 8 data bits
  v2 = val & 0xFF;

  // Delay to have CS high for a short while
  short_wait();

  // Enable SPI: Use CS 1 and set activate bit
  SPI0_CNTLSTAT = SPI0_CS_CHIPSEL1|SPI0_CS_ACTIVATE;

  // send the values
  SPI0_FIFO = v1;
  SPI0_FIFO = v2;

  // wait for SPI to be ready
  // This will take about 16 micro seconds
  do {
     status = SPI0_CNTLSTAT;
  } while ((status & SPI0_CS_DONE)==0);
  SPI0_CNTLSTAT = SPI0_CS_DONE; // clear the done bit

  // For every transmit there is also data coming back
  // We MUST read that received data from the FIFO
  // even if we do not use it!
  dummy = SPI0_FIFO;
  dummy = SPI0_FIFO;
} // write_dac

