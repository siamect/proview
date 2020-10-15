#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

import sys
import math
import pandas as pd
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
    # pwrrt.getSevItemList()
    #
    def getSevItemList(self):
        d = ('VolPwrTest01d:Test01d-H1-Av1.ActualValue',
             'VolPwrTest01d:Test01d-H1-Av2.ActualValue')
            
        try:
            self.itemlist = pwrrt.getSevItemList('localhost', '')
            for i in range(len(d)):
                 name = self.itemlist[i][0] + '.' + self.itemlist[i][6]
                 if name != d[i]:
                    self.logger.vlog('E', "getSevItemList, Name doesn't match, %s != %s, idx %s",
                                     name, d[i][0], str(i))
                    return

        except RuntimeError as e:
            self.logger.vlog('E', 'getSevItemList, Unexpected exception %s',
                                     str(e))
            return
        self.logger.log('S', 'sevGetItemList, successfull completion')

    #
    # pwrrt.getSevItemData()
    #
    def getSevItemData(self):
        try:
            result = pwrrt.getSevItemData( 'localhost', self.itemlist[0][1], self.itemlist[0][6],
                                            '00:02:00', 'now', 1000)
            ser = pd.Series(result[1])
            mean = ser.mean()
            std = ser.std()
            if len(ser) < 119 or len(result) > 121:
                self.logger.vlog('E', "getSevItemData, number of rows error, %s",
                                 str(len(result)))
            if mean < 49 or mean > 51:
                self.logger.vlog('E', "getSevItemData, mean error, %s, idx %s",
                                 str(mean), str(0))
                return
            if std < 34 or std > 36:
                self.logger.vlog('E', "getSevItemData, std error, %s, idx %s",
                                 str(std), str(0))
                return
        except RuntimeError as e:
            self.logger.vlog('E', 'getSevItemData, Unexpected exception %s',
                                     str(e))
            return
        self.logger.log('S', 'sevGetSevItemData, successfull completion')

    #
    # pwrrt.getSevItemsDataFrame()
    #
    def getSevItemsDataFrame(self):
        oidlist = []
        attrlist = []
        isobjectlist = []
        for i in range(2):            
            oidlist.append(self.itemlist[i][0])
            attrlist.append(self.itemlist[i][6])
            isobjectlist.append(0)
        try:
            result = pwrrt.getSevItemsDataFrame( 'localhost', oidlist, attrlist, isobjectlist,
                                                 '00:02:00', 'now', 0.5, 1000)
            columns = ('time', 'A1', 'A2')
            data = pd.DataFrame(data=result)
            data.columns = columns
            mean = []
            std = []
            for i in range(2):
                mean.append(data[columns[i+1]].mean())
                std.append(data[columns[i+1]].std())
            if len(result) < 238 or len(result) > 242:
                self.logger.vlog('E', "getSevItemsDataFrame, number of rows error, %s",
                                 str(len(result)))
            if mean[0] < 47 or mean[0] > 53:
                self.logger.vlog('E', "getSevItemsDataFrame, mean error, %s, idx %s",
                                 str(mean[0]), str(0))
                return
            if std[0] < 34 or std[0] > 36:
                self.logger.vlog('E', "getSevItemsDataFrame, std error, %s, idx %s",
                                 str(std[0]), str(0))
                return
            if mean[1] < 47 or mean[1] > 53:
                self.logger.vlog('E', "getSevItemsDataFrame, mean error, %s, idx %s",
                                 str(mean[1]), str(1))
                return
            if std[1] < 27 or std[1] > 29:
                self.logger.vlog('E', "getSevItemsDataFrame, std error, %s, idx %s",
                                 str(std[1]), str(1))
                return
        except RuntimeError as e:
            self.logger.vlog('E', 'getSevItemsDataFrame, Unexpected exception %s',
                                     str(e))
            return
        self.logger.log('S', 'sevGetSevItemsDataFrame, successfull completion')

    #
    # pwrrt.getSevItemsDataFrameD()
    #
    def getSevItemsDataFrameD(self):
        oidlist = []
        attrlist = []
        isobjectlist = []
        for i in range(2):            
            oidlist.append(self.itemlist[i][0])
            attrlist.append(self.itemlist[i][6])
            isobjectlist.append(0)
        try:
            result = pwrrt.getSevItemsDataFrameD( 'localhost', oidlist, attrlist, isobjectlist,
                                                 '00:02:00', 'now', 0.5, 1000)
            columns = ('time', 'A1', 'A2')
            data = pd.DataFrame(data=result)
            data.columns = columns
            mean = []
            std = []
            for i in range(2):
                mean.append(data[columns[i+1]].mean())
                std.append(data[columns[i+1]].std())
            print data[columns[1]].mean()
            if len(result) < 238 or len(result) > 242:
                self.logger.vlog('E', "getSevItemsDataFrame, number of rows error, %s",
                                 str(len(result)))
            if mean[0] < 47 or mean[0] > 53:
                self.logger.vlog('E', "getSevItemsDataFrameD, mean error, %s, idx %s",
                                 str(mean[0]), str(0))
                print data[columns[1]]
                return
            if std[0] < 34 or std[0] > 36:
                self.logger.vlog('E', "getSevItemsDataFrameD, std error, %s, idx %s",
                                 str(std[0]), str(0))
                return
            if mean[1] < 47 or mean[1] > 53:
                print data[columns[2]]
                self.logger.vlog('E', "getSevItemsDataFrameD, mean error, %s, idx %s",
                                 str(mean[1]), str(1))
                return
            if std[1] < 27 or std[1] > 29:
                self.logger.vlog('E', "getSevItemsDataFrameD, std error, %s, idx %s",
                                 str(std[1]), str(1))
                return
        except RuntimeError as e:
            self.logger.vlog('E', 'getSevItemsDataFrameD, Unexpected exception %s',
                                     str(e))
            return
        self.logger.log('S', 'sevGetSevItemsDataFrameD, successfull completion')

    #
    # pwrrt.getSevEvents()
    #
    def getSevEvents(self):
        try:
            i = 0
            found = 0
            for item in self.itemlist:
                if item[6] == 'Events':
                    found = 1
                    break
                i += 1

            result = pwrrt.getSevEvents( 'localhost', self.itemlist[i][1],
                                         '20:00:00', 'now',  8001, 15, '', '', 1000)
            type = [row[1] for row in result]
            print type
            count = type.count(64)  # Number of A alarms
            if  count != 10:
                self.logger.vlog('E', "getSevEvents, no of A alarms, %s",
                                 str(count))
                return
            # Number of returns
            count = type.count(7)  # Number of Returns
            if  count != 10:
                self.logger.vlog('E', "getSevEvents, no of returns, %s",
                                 str(count))
                return

        except RuntimeError as e:
            self.logger.vlog('E', 'getSevEvents, Unexpected exception %s',
                             str(e))
            return
        self.logger.log('S', 'sevGetSevEvents, successfull completion')

    #
    # pwrrt.getSevEventsDataFrame()
    #
    def getSevEventsDataFrame(self):
        try:
            i = 0
            found = 0
            for item in self.itemlist:
                if item[6] == 'Events':
                    found = 1
                    break
                i += 1

            result = pwrrt.getSevEventsDataFrame( 'localhost', self.itemlist[i][1],
                                            '20:00:00', 'now',  8001, 15, '', '', 1000)
            data = pd.DataFrame(data=result)
            columns = ('Time', 'Type', 'Prio', 'DetectText', 'Name', 'Oid', 'Id', 'Sts')
            data.columns = columns
            count = len(data[data.Type == 64])  # Number of A alarms
            if  count != 10:
                self.logger.vlog('E', "getSevEventsDataFrame, no of A alarms, %s",
                                 str(count))
                return
            # Number of returns
            count = len(data[data.Type == 7])   # Number of Returns
            if  count != 10:
                self.logger.vlog('E', "getSevEventsDataFrame, no of returns, %s",
                                 str(count))
                return

        except RuntimeError as e:
            self.logger.vlog('E', 'getSevEventsDataFrame, Unexpected exception %s',
                             str(e))
            return
        self.logger.log('S', 'sevGetSevEventsDataFrame, successfull completion')

    #
    # Oid()
    #
    def Oid(self):
        d = (
            ('MDyn-H1-Av1', '1_254_254_204:pwrNode-sev-Test01d-H1-Av1', ''),
            ('MDyn-H1-Av2', '1_254_254_204:pwrNode-sev-Test01d-H1-Av2', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.Oid(d[i][0])
                name = o.fullName()
                if name != d[i][1]:
                    self.logger.vlog('E', "Oid, Name doesn't match, %s != %s, idx %s",
                                    name, d[i][1], str(i))
                    return

            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    print 'Not correct error code'
                    return
                
        self.logger.log('S', 'Oid, successfull completion')

    def Oid_attribute(self):
        d = (
            ('MDyn-H1-Av1', 'ActualValue', '1_254_254_204:pwrNode-sev-Test01d-H1-Av1-ActualValue.Value'),
            ('MDyn-H1-Av2', 'ActualValue', '1_254_254_204:pwrNode-sev-Test01d-H1-Av2-ActualValue.Value'),
            ('MDyn-H1-Iv1', 'ActualValue', '1_254_254_204:pwrNode-sev-Test01d-H1-Iv1-ActualValue.Value'),
            ('MDyn-H1-Sv1', 'ActualValue', '1_254_254_204:pwrNode-sev-Test01d-H1-Sv1-ActualValue.Value')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                a = o.attribute(d[i][1])
                if a.fullName() != d[i][2]:
                    self.logger.vlog('E', "Oid.attribute, "
                                     "Name doesn't match, %s != %s",
                                     a.name(), d[i][2])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid.attribute, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Oid.attribute, successfull completion')
    
    #
    # Aref()
    #
    def Aref(self):
        d = (
            ('MDyn-H1-Av3.ActualValue', 22.22, ''),
            ('MDyn-H1-Av4.ActualValue', 33.33, ''),
            ('MDyn-H1-Iv3.ActualValue', 44, ''),
            ('MDyn-H1-Dv7.ActualValue', 1, ''),
            ('MDyn-H1-Sv1.ActualValue', 'String value 1', ''),
            ('MDyn-H1-Values.Int32', 4444, ''),
            ('MDyn-H1-Values.Int32Array[2]', 3, ''),
            ('MDyn-H1-Values.Float32Array[4]', 5.0, ''),
            ('MDyn-H1-Values.String80Array[3]', '4', ''),
            ('MDyn-H1-Values2.Int32', 4444, ''),
            ('MDyn-H1-Values2.UInt64', 99999, ''),
            ('MDyn-H1-Av1.NoSuchAttr', 22.22, '%GDH-E-NODYNLOCOBJ, noexisting dynamic local object'),
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.Aref(d[i][0])
                value = a.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "Aref, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "Aref, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Aref, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref, successfull completion')

    #
    # Aref.tid()
    #
    def Aref_tid(self):
        d = (
            ('MDyn-H1-Av1.ActualValue', 'pwrs:Type-$Float32', ''),
            ('MDyn-H1-Av2.ActualValue', 'pwrs:Type-$Float32', ''),
            ('MDyn-H1-Iv1.ActualValue', 'pwrs:Type-$Int32', ''),
            ('MDyn-H1-Dv5.ActualValue', 'pwrs:Type-$Boolean', ''),
            ('MDyn-H1-Sv1.ActualValue', 'pwrs:Type-$String80', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                tid = a.tid()
                if tid.fullName() != d[i][1]:
                    self.logger.vlog('E', "Aref.tid, "
                                     "Name doesn't match, %s != %s",
                                     tid.fullName(), d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Aref.tid, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref.tid, successfull completion')
    

ctx = Ctx()
ctx.logger = pwrtest.logger('rt-sev', '$pwrp_log/sev.tlog')
try:
    ctx.init()
except:
    quit()
pwrrt.login('pwrp', 'pwrp')
ctx.getSevItemList()
ctx.getSevItemData()
ctx.getSevItemsDataFrame()
ctx.getSevItemsDataFrameD()
ctx.getSevEvents()
ctx.getSevEventsDataFrame()
ctx.Oid()
ctx.Oid_attribute()
ctx.Aref()
ctx.Aref_tid()
