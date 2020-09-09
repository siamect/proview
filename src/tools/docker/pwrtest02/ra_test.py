#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

import sys
import math
import time
import pwrrt
import pwrtest

class Ctx:

    #
    # Static pwrrt methods
    #

    #
    # pwrrt.init()
    #
    def init(self):
        try:                
            pwrrt.init('ra_pwrrttest')
        except RuntimeError as e:
            self.logger.vlog('E', 'init, Unexpected exception %s', str(e))
            raise e
        self.logger.log('S', 'init, successfull completion')

            
    #
    # Check system status
    #
    def systemStatus(self):
        a = pwrrt.attribute('Nodes-Pwrtest02.SystemStatus')
        sts = a.value()
        msg = pwrrt.getMsg(sts)
        if sts % 2 == 0:
            self.logger.log('E', 'System status', msg);
            return
        self.logger.log('S', 'System status', msg)

    #
    # Check plc threads
    #
    def plcThreads(self):
        d = (
            ((1, 'PlcTest-H1-Dv1.ActualValue', 1),
             (1, 'PlcTest-H1-Dv2.ActualValue', 1),
             (1, 'PlcTest-H1-Dv3.ActualValue', 0),
             (0, 'PlcTest-H1-Dv4.ActualValue', 1)),
            ((1, 'PlcTest-H2-Dv1.ActualValue', 1),
             (1, 'PlcTest-H2-Dv2.ActualValue', 1),
             (1, 'PlcTest-H2-Dv3.ActualValue', 0),
             (0, 'PlcTest-H2-Dv4.ActualValue', 1)),
            ((1, 'PlcTest-H3-Dv1.ActualValue', 1),
             (1, 'PlcTest-H3-Dv2.ActualValue', 1),
             (1, 'PlcTest-H3-Dv3.ActualValue', 0),
             (0, 'PlcTest-H3-Dv4.ActualValue', 1)),
            ((1, 'PlcTest-H4-Dv1.ActualValue', 1),
             (1, 'PlcTest-H4-Dv2.ActualValue', 1),
             (1, 'PlcTest-H4-Dv3.ActualValue', 0),
             (0, 'PlcTest-H4-Dv4.ActualValue', 1))
            )
        for i in range(len(d)):
            for j in range(len(d[i])):
                if d[i][j][0] == 1:
                    a = pwrrt.attribute(d[i][j][1])
                    a.setValue(d[i][j][2])

        time.sleep(0.1)
        for i in range(len(d)):
            for j in range(len(d[i])):
                if d[i][j][0] == 0:
                    a = pwrrt.attribute(d[i][j][1])
                    val = a.value()
                    if val != d[i][j][2]:
                        self.logger.vlog('E', 'Plc thread %s, test value mismatch', str(i))
                    else:
                        self.logger.vlog('S', 'PlcThread %s test ok', str(i))



ctx = Ctx()
ctx.logger = pwrtest.logger('wbtest-rt', '$pwrp_log/pwrtest02_rt.tlog')
try:
    ctx.init()
except:
    quit()
pwrrt.login('pwrp', 'pwrp')

ctx.systemStatus()
ctx.plcThreads()
