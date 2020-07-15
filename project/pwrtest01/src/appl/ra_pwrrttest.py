#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

import sys
import math
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
    # pwrrt.login()/logout()
    #
    def login(self):
        d = (
            ('pwrp', 'no', 0, '%USER-E-NOTAUTHORIZED, user is not authorized'),
            ('no', 'pwrp', 0, '%USER-E-NOSUCHUSER, no such user'),
            ('pwrp', '', 0, '%USER-E-NOTAUTHORIZED, user is not authorized'),
            ('pwrp', 'pwrp', 167510022, '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                pwrrt.login(d[i][0], d[i][1])
                user = pwrrt.getUser()
                priv = pwrrt.getPriv()
                if user != d[i][0]:
                    self.logger.vlog('E', 'login, getUser, wrong user %s, %s',
                                     user, d[i][1])
                    pwrrt.logout()
                    return
                if priv != d[i][2]:
                    self.logger.vlog('E', 'login, getPriv, wrong priv %s, %s',
                                     str(priv), str(d[i][2]))
                    pwrrt.logout()
                    return
                pwrrt.logout()
            except RuntimeError as e:
                if str(e) != d[i][3]:
                    self.logger.vlog('E', 'login, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return

        self.logger.log('S', 'login, successfull completion')
            
    #
    # pwrrt.volume()
    #
    def volume(self):
        d = (
            ('pwrs', ''),
            ('pwrb', ''),
            ('VolPwrTest01c', ''),
            ('CVolPwrtest01', ''),
            ('1_254_254_203', ''),
            ('rt', ''),
            ('VolNoVolume', '%GDH-E-NOSUCHVOL, no such volume exists'),
            )
        i = 0
        for i in range(len(d)):
            try:                
                vol = pwrrt.volume(d[i][0])
                if vol.name() != d[i][0]:
                    self.logger.vlog('E', "volume, Name doesn't match, %s != %s, idx %s",
                                    vol.name(), d[i][0], str(i))
                    return
            except RuntimeError as e:
                if str(e) != d[i][1]:
                    self.logger.vlog('E', 'volume, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'volume, successfull completion')

    #
    # pwrrt.volumes()
    #
    def volumes(self):
        d = ('VolPwrTest01c', 'rt', 'pwrs', 'pwrb', 'Simul', 'BaseComponent',
             'NMps', 'Profibus', 'CVolPwrtest01', '1_254_254_203')
        i = 0
        try:                
           vol = pwrrt.volumes()
           for v in vol:
               if v.name() != d[i]:
                   self.logger.vlog('E', "volumes, Name doesn't match, %s != %s",
                                    v.name(), d[i])
                   return
               i += 1
        except RuntimeError as e:
            self.logger.vlog('E', 'volumes, Unexpected exception %s, idx %s',
                                     str(e), str(i))
            return
        self.logger.log('S', 'volumes, successfull completion')

    #
    # pwrrt.object()
    #
    def object(self):
        d = (
            ('Test01c-Pwrrt-Av1', 'VolPwrTest01c:Test01c-Pwrrt-Av1', ''),
            ('VolPwrTest01c:Test01c-Pwrrt-Av1', 'VolPwrTest01c:Test01c-Pwrrt-Av1', ''),
            ('Test01c-Pwrrt-Dv1', 'VolPwrTest01c:Test01c-Pwrrt-Dv1', ''),
            ('Test01c-Pwrrt-Dv2', 'VolPwrTest01c:Test01c-Pwrrt-Dv2', ''),
            ('Test01c-Pwrrt-L1', 'VolPwrTest01c:Test01c-Pwrrt-L1', ''),
            ('Test01c-Pwrrt', 'VolPwrTest01c:Test01c-Pwrrt', ''),
            ('Test01c', 'VolPwrTest01c:Test01c', ''),
            ('Test01c-Pwrrt-NoObject', '', '%HASH-E-NOTFOUND, entry not found'),
            ('VolPwrTest01c:', 'VolPwrTest01c:', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                name = o.fullName()
                if name != d[i][1]:
                    self.logger.vlog('E', "object, Name doesn't match, %s != %s, idx %s",
                                    name, d[i][1], str(i))
                    return

            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'object, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    print 'Not correct error code'
                    return
                
        self.logger.log('S', 'object, successfull completion')

    #
    # pwrrt.attribute()
    #
    def attribute(self):
        d = (
            ('Test01c-Pwrrt-Av1.ActualValue', 22.22, ''),
            ('Test01c-Pwrrt-Av1.NoSuchAttr', 22.22, '%GDH-E-ATTRIBUTE, no such attribute'),
            ('Test01c-Pwrrt-Dv1.ActualValue', 0, ''),
            ('Test01c-Pwrrt-Dv2.ActualValue', 1, ''),
            ('Test01c-Pwrrt-L1.Value.ActualValue', 33.33, ''),
            ('Test01c-Pwrrt-L1.LimitHH.Limit', 95, ''),
            ('Test01c-Pwrrt-L1.Description', 'BaseLevelSensor used for python tests', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                value = a.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "attribute, Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "attribute, Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'attribute, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'attribute, successfull completion')

    #
    # pwrrt.subscribe()
    #
    def subscribe(self):
        d = (
            ('Test01c-Pwrrt-Av1.ActualValue', 22.22, ''),
            ('Test01c-Pwrrt-Av1.NoSuchAttr', 22.22, '%GDH-E-ATTRIBUTE, no such attribute'),
            ('Test01c-Pwrrt-Dv1.ActualValue', 0, ''),
            ('Test01c-Pwrrt-Dv2.ActualValue', 1, ''),
            ('Test01c-Pwrrt-L1.Value.ActualValue', 33.33, ''),
            ('Test01c-Pwrrt-L1.LimitHH.Limit', 95, ''),
            ('Test01c-Pwrrt-L1.Description', 'BaseLevelSensor used for python tests', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                sub = pwrrt.subscribe(d[i][0])
                value = sub.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "subscribe, Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "subscribe, Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
                sub.close();
            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'subscribe, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'subscribe, successfull completion')


    #
    # Oid functions
    #

    #
    # Oid()
    #
    def Oid(self):
        d = (
            ('Test01c-Pwrrt-Av1', 'VolPwrTest01c:Test01c-Pwrrt-Av1', ''),
            ('VolPwrTest01c:Test01c-Pwrrt-Av1', 'VolPwrTest01c:Test01c-Pwrrt-Av1', ''),
            ('Test01c-Pwrrt-Dv1', 'VolPwrTest01c:Test01c-Pwrrt-Dv1', ''),
            ('Test01c-Pwrrt-Dv2', 'VolPwrTest01c:Test01c-Pwrrt-Dv2', ''),
            ('Test01c-Pwrrt-L1', 'VolPwrTest01c:Test01c-Pwrrt-L1', ''),
            ('Test01c-Pwrrt', 'VolPwrTest01c:Test01c-Pwrrt', ''),
            ('Test01c', 'VolPwrTest01c:Test01c', ''),
            ('Test01c-Pwrrt-NoObject', '', '%HASH-E-NOTFOUND, entry not found'),
            ('VolPwrTest01c:', 'VolPwrTest01c:', '')
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

    #
    # Oid.child()
    #
    def Oid_child(self):
        d = (
            ('RootObject', 'Child1'),
            ('RootObject-Child1', 'A1'),
            ('RootObject-Child1-A1', ''),
            ('RootObject-ChildLess','')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                child = o.child()
                if child is None:
                    if d[i][1] != '':
                        self.logger.vlog('E', 'Oid.child, child is missing, idx %s',
                                         str(i))
                else:            
                    name = child.name()
                    if name != d[i][1]:
                        self.logger.vlog('E', "Oid.child, name doesn't match, %s != %s, idx %s",
                                         name, d[i][1], str(i))
                        return

            except RuntimeError as e:
                self.logger.vlog('E', 'Oid.child, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Oid.child, successfull completion')
    
    #
    # Oid.next()
    #
    def Oid_next(self):
        d = (
            ('RootObject-Child1', 'Child2'),
            ('RootObject-Child2', 'Child3'),
            ('RootObject-Child3', 'Child4'),
            ('RootObject-Child4', 'ChildLess'),
            ('RootObject-ChildLess', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                next = o.next()
                if next is None:
                    if d[i][1] != '':
                        self.logger.vlog('E', 'Oid.next, next is missing, idx %s',
                                         str(i))
                else:            
                    name = next.name()
                    if name != d[i][1]:
                        self.logger.vlog('E', "Oid.next, name doesn't match, %s != %s, idx %s",
                                         name, d[i][1], str(i))
                        return

            except RuntimeError as e:
                self.logger.vlog('E', 'Oid.next, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Oid.next, successfull completion')
    
    #
    # Oid.parent()
    #
    def Oid_parent(self):
        d = (
            ('RootObject-Child1-A1', 'Child1', ''),
            ('RootObject-Child1', 'RootObject', ''),
            ('RootObject', '', ''),
            ('NoSuchObject', '', '%HASH-E-NOTFOUND, entry not found')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                parent = o.parent()
                if parent is None:
                    if d[i][1] != '':
                        self.logger.vlog('E', 'Oid.parent, parent is missing, idx %s',
                                         str(i))
                else:            
                    name = parent.name()
                    if name != d[i][1]:
                        self.logger.vlog('E', "Oid.parent, name doesn't match, %s != %s, idx %s",
                                         name, d[i][1], str(i))
                        return

            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid.parent, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Oid.parent, successfull completion')
    
    #
    # Oid.children()
    #
    def Oid_children(self):
        d = (
            ('RootObject', ('Child1','Child2','Child3','Child4','ChildLess'), ''),
            ('RootObject-Child1', ('A1','A2','P1'), ''),
            ('RootObject-ChildLess', (), '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                children = o.children()

                j = 0
                for c in children:
                    if c.name() != d[i][1][j]:
                        self.logger.vlog('E', "Oid.children, Name doesn't match, %s != %s",
                                         c.name(), d[i][1][j])
                        return
                    j += 1


            except RuntimeError as e:
                self.logger.vlog('E', 'Oid.children, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Oid.children, successfull completion')
    
    #
    # Oid.name()
    #
    def Oid_name(self):
        d = (
            ('RootObject', 'RootObject', ''),
            ('RootObject-Child1', 'Child1', ''),
            ('Test01c-Gdh-LongName90123456789012345678901', 'LongName90123456789012345678901', ''),
            ('Test01c-Gdh-ÄÅÖäåö', 'ÄÅÖäåö', ''),
            ('Test01c-Gdh-sdf*sdf', '', '%CDH-E-INVCHAR, invalid character'),
            ('VolPwrTest01c:', 'VolPwrTest01c', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                name = o.name()
                if name != d[i][1]:
                    self.logger.vlog('E', "Oid.name, Name doesn't match, %s != %s",
                                     name, d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid.name, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Oid.name, successfull completion')
    
    #
    # Oid.fullName()
    #
    def Oid_fullName(self):
        d = (
            ('RootObject', 'VolPwrTest01c:RootObject', ''),
            ('RootObject-Child1', 'VolPwrTest01c:RootObject-Child1', ''),
            ('Test01c-Gdh-LongName90123456789012345678901', 'VolPwrTest01c:Test01c-Gdh-LongName90123456789012345678901', ''),
            ('Test01c-Gdh-ÄÅÖäåö', 'VolPwrTest01c:Test01c-Gdh-ÄÅÖäåö', ''),
            ('Test01c-Gdh-sdf*sdf', '', '%CDH-E-INVCHAR, invalid character'),
            ('VolPwrTest01c:', 'VolPwrTest01c:', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                name = o.fullName()
                if name != d[i][1]:
                    self.logger.vlog('E', "Oid.fullName, Name doesn't match, %s != %s",
                                     name, d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid.fullName, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Oid.fullName, successfull completion')
    
    #
    # Oid.oidStr()
    #
    def Oid_oidStr(self):
        d = (
            ('RootObject', '_O0.254.254.203:12147', ''),
            ('RootObject-Child1', '_O0.254.254.203:12149', ''),
            ('VolPwrTest01c:', '_O0.254.254.203:0', ''),
            ('pwrb:Class-And', '_O0.0.0.2:2149842944', ''),
            ('pwrs:', '_O0.0.0.1:0', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                name = o.oidStr()
                if name != d[i][1]:
                    self.logger.vlog('E', "Oid.oidStr, Name doesn't match, %s != %s",
                                     name, d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid.oidStr, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Oid.oidStr, successfull completion')
    
    #
    # Oid.cid()
    #
    def Oid_cid(self):
        d = (
            ('RootObject', '$PlantHier', ''),
            ('RootObject-Child1', '$PlantHier', ''),
            ('Test01c-Pwrrt-Dv1', 'Dv', ''),
            ('Test01c-Pwrrt-L1', 'BaseLevelSensor', ''),
            ('VolPwrTest01c:', '$RootVolume', ''),
            ('pwrb:Class-And', '$ClassDef', ''),
            ('pwrs:', '$ClassVolume', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                cid = o.cid()
                if cid.name() != d[i][1]:
                    self.logger.vlog('E', "Oid.cid, Name doesn't match, %s != %s",
                                     cid.name(), d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid.cid, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Oid.cid, successfull completion')
    
    #
    # Oid.attribute()
    #
    def Oid_attribute(self):
        d = (
            ('RootObject', 'Description', 'RootObject.Description'),
            ('RootObject-Child1', 'Description', 'Child1.Description'),
            ('Test01c-Pwrrt-Dv1', 'ActualValue', 'Dv1.ActualValue'),
            ('Test01c-Pwrrt-L1', 'LimitLL', 'L1.LimitLL'),
            ('Test01c-Pwrrt-L1', 'Value.ActualValue', 'L1.Value.ActualValue')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrrt.object(d[i][0])
                a = o.attribute(d[i][1])
                if a.name() != d[i][2]:
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
    # Aref functions
    #
    
    #
    # Aref()
    #
    def Aref(self):
        d = (
            ('Test01c-Pwrrt-Av1.ActualValue', 22.22, ''),
            ('Test01c-Pwrrt-Av1.NoSuchAttr', 22.22, '%GDH-E-ATTRIBUTE, no such attribute'),
            ('Test01c-Pwrrt-Dv1.ActualValue', 0, ''),
            ('Test01c-Pwrrt-Dv2.ActualValue', 1, ''),
            ('Test01c-Pwrrt-L1.Value.ActualValue', 33.33, ''),
            ('Test01c-Pwrrt-L1.LimitHH.Limit', 95, ''),
            ('Test01c-Pwrrt-L1.Description', 'BaseLevelSensor used for python tests', '')
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
    # Aref.name()
    #
    def Aref_name(self):
        d = (
            ('RootObject.Description', 'RootObject.Description', ''),
            ('RootObject-Child1.DefGraph', 'Child1.DefGraph', ''),
            ('Test01c-Gdh-LongName90123456789012345678901.InitialValue', 'LongName90123456789012345678901.InitialValue', ''),
            ('Test01c-Gdh-ÄÅÖäåö.ValueIndex', 'ÄÅÖäåö.ValueIndex', ''),
            ('Test01c-Gdh-sdf*sdf', '', '%CDH-E-INVCHAR, invalid character')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                name = a.name()
                if name != d[i][1]:
                    self.logger.vlog('E', "Aref.name, Name doesn't match, %s != %s",
                                     name, d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Aref.name, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref.name, successfull completion')
    
    #
    # Aref.fullName()
    #
    def Aref_fullName(self):
        d = (
            ('RootObject.Description', 'VolPwrTest01c:RootObject.Description', ''),
            ('RootObject-Child1.DefGraph', 'VolPwrTest01c:RootObject-Child1.DefGraph', ''),
            ('Test01c-Gdh-LongName90123456789012345678901.InitialValue', 'VolPwrTest01c:Test01c-Gdh-LongName90123456789012345678901.InitialValue', ''),
            ('Test01c-Gdh-ÄÅÖäåö.ValueIndex', 'VolPwrTest01c:Test01c-Gdh-ÄÅÖäåö.ValueIndex', ''),
            ('Test01c-Gdh-sdf*sdf', '', '%CDH-E-INVCHAR, invalid character')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                name = a.fullName()
                if name != d[i][1]:
                    self.logger.vlog('E', "Aref.fullName, "
                                     "Name doesn't match, %s != %s",
                                     name, d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Aref.fullName, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref.fullName, successfull completion')
    
    #
    # Aref.arefStr()
    #
    def Aref_arefStr(self):
        d = (
            ('RootObject', '_A0.254.254.203:12147(_T0.1:0.17.1)[0.656]', ''),
            ('RootObject-Child1', '_A0.254.254.203:12149(_T0.1:0.17.1)[0.656]', ''),
            ('VolPwrTest01c:', '_A0.254.254.203:0(_T0.1:0.47.1)[0.160]', ''),
            ('pwrb:Class-And', '_A0.0.0.2:2149842944(_T0.1:0.1.1)[0.24]', ''),
            ('pwrs:', '_A0.0.0.1:0(_T0.1:0.52.1)[0.208]', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                name = a.arefStr()
                if name != d[i][1]:
                    self.logger.vlog('E', "Aref.arefStr, "
                                     "Name doesn't match, %s != %s",
                                     name, d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Aref.arefStr, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref.arefStr, successfull completion')
    
    #
    # Aref.tid()
    #
    def Aref_tid(self):
        d = (
            ('RootObject.Description', 'pwrs:Type-$String80', ''),
            ('RootObject-Child1.DefGraph', 'pwrs:Type-$AttrRef', ''),
            ('Test01c-Pwrrt-Dv1.InitialValue', 'pwrs:Type-$Boolean', ''),
            ('Test01c-Pwrrt-L1.Value', 'pwrb:Class-Ai', ''),
            ('Test01c-Pwrrt-L1.Value.ActualValue', 'pwrs:Type-$Float32', ''),
            ('Test01c-Pwrrt-Values.Boolean', 'pwrs:Type-$Boolean', ''),
            ('Test01c-Pwrrt-Values.Int8', 'pwrs:Type-$Int8', ''),
            ('Test01c-Pwrrt-Values.Int16', 'pwrs:Type-$Int16', ''),
            ('Test01c-Pwrrt-Values.Int32', 'pwrs:Type-$Int32', ''),
            ('Test01c-Pwrrt-Values.Int64', 'pwrs:Type-$Int64', ''),
            ('Test01c-Pwrrt-Values.UInt8', 'pwrs:Type-$UInt8', ''),
            ('Test01c-Pwrrt-Values.UInt16', 'pwrs:Type-$UInt16', ''),
            ('Test01c-Pwrrt-Values.UInt32', 'pwrs:Type-$UInt32', ''),
            ('Test01c-Pwrrt-Values.UInt64', 'pwrs:Type-$UInt64', ''),
            ('Test01c-Pwrrt-Values.Float32', 'pwrs:Type-$Float32', ''),
            ('Test01c-Pwrrt-Values.Float64', 'pwrs:Type-$Float64', ''),
            ('Test01c-Pwrrt-Values.String80', 'pwrs:Type-$String80', ''),
            ('Test01c-Pwrrt-Values.Time', 'pwrs:Type-$Time', ''),
            ('Test01c-Pwrrt-Values.DeltaTime', 'pwrs:Type-$DeltaTime', ''),
            ('Test01c-Pwrrt-Values.Objid', 'pwrs:Type-$Objid', ''),
            ('Test01c-Pwrrt-Values.AttrRef', 'pwrs:Type-$AttrRef', '')
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
    
    #
    # Aref.value()
    #
    def Aref_value(self):
        d = (
            ('Test01c-Pwrrt-Av1.ActualValue', 22.22, ''),
            ('Test01c-Pwrrt-Av1.NoSuchAttr', 22.22, '%GDH-E-ATTRIBUTE, no such attribute'),
            ('Test01c-Pwrrt-Dv1.ActualValue', 0, ''),
            ('Test01c-Pwrrt-Dv2.ActualValue', 1, ''),
            ('Test01c-Pwrrt-L1.Value.ActualValue', 33.33, ''),
            ('Test01c-Pwrrt-L1.LimitHH.Limit', 95, ''),
            ('Test01c-Pwrrt-L1.Description', 'BaseLevelSensor used for python tests', ''),
            ('Test01c-Pwrrt-Values.Boolean', 1, ''),
            ('Test01c-Pwrrt-Values.Int8', 22, ''),
            ('Test01c-Pwrrt-Values.Int16', 333, ''),
            ('Test01c-Pwrrt-Values.Int32', 4444, ''),
            ('Test01c-Pwrrt-Values.Int64', 55555, ''),
            ('Test01c-Pwrrt-Values.UInt8', 66, ''),
            ('Test01c-Pwrrt-Values.UInt16', 777, ''),
            ('Test01c-Pwrrt-Values.UInt32', 8888, ''),
            ('Test01c-Pwrrt-Values.UInt64', 99999, ''),
            ('Test01c-Pwrrt-Values.Float32', 22.22, ''),
            ('Test01c-Pwrrt-Values.Float64', 4444.4444, ''),
            ('Test01c-Pwrrt-Values.String80', "Why don't you have wings to fly with?", ''),
            ('Test01c-Pwrrt-Values.Time', '15-MAY-2020 08:27:50.50', ''),
            ('Test01c-Pwrrt-Values.DeltaTime', '27 8:27:50.05', ''),
            ('Test01c-Pwrrt-Values.Objid', 'VolPwrTest01c:Test01c-Pwrrt-Values', ''),
            ('Test01c-Pwrrt-Values.AttrRef', 'VolPwrTest01c:Test01c-Pwrrt-Values.AttrRef', ''),
            ('Test01c-Pwrrt-Values.BooleanArray[4]', 1, ''),
            ('Test01c-Pwrrt-Values.Int8Array[4]', 22, ''),
            ('Test01c-Pwrrt-Values.Int16Array[4]', 333, ''),
            ('Test01c-Pwrrt-Values.Int32Array[4]', 4444, ''),
            ('Test01c-Pwrrt-Values.Int64Array[4]', 55555, ''),
            ('Test01c-Pwrrt-Values.UInt8Array[4]', 66, ''),
            ('Test01c-Pwrrt-Values.UInt16Array[4]', 777, ''),
            ('Test01c-Pwrrt-Values.UInt32Array[4]', 8888, ''),
            ('Test01c-Pwrrt-Values.UInt64Array[4]', 99999, ''),
            ('Test01c-Pwrrt-Values.Float32Array[4]', 22.22, ''),
            ('Test01c-Pwrrt-Values.Float64Array[4]', 4444.4444, ''),
            ('Test01c-Pwrrt-Values.String80Array[4]', "Why don't you have wings to fly with?", ''),
            ('Test01c-Pwrrt-Values.TimeArray[4]', '15-MAY-2020 08:27:50.50', ''),
            ('Test01c-Pwrrt-Values.DeltaTimeArray[4]', '27 8:27:50.05', ''),
            ('Test01c-Pwrrt-Values.ObjidArray[4]', 'VolPwrTest01c:Test01c-Pwrrt-Values', ''),
            ('Test01c-Pwrrt-Values.AttrRefArray[4]', 'VolPwrTest01c:Test01c-Pwrrt-Values.AttrRefArray[4]', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                value = a.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "Aref.value, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                elif type(value).__name__ == 'Oid' or type(value).__name__ == 'Aref' :
                    if value.fullName() != d[i][1]:
                        self.logger.vlog('E', "Aref.value, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value.fullName(), d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "Aref.value, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'aref.value, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref.value, successfull completion')


    #
    # Aref.setValue()
    #
    def Aref_setValue(self):
        d = (
            ('Test01c-Pwrrt-SetValues.Boolean', 1, ''),
            ('Test01c-Pwrrt-SetValues.Int8', 22, ''),
            ('Test01c-Pwrrt-SetValues.Int16', 333, ''),
            ('Test01c-Pwrrt-SetValues.Int32', 4444, ''),
            ('Test01c-Pwrrt-SetValues.Int64', 55555, ''),
            ('Test01c-Pwrrt-SetValues.UInt8', 66, ''),
            ('Test01c-Pwrrt-SetValues.UInt16', 777, ''),
            ('Test01c-Pwrrt-SetValues.UInt32', 8888, ''),
            ('Test01c-Pwrrt-SetValues.UInt64', 99999, ''),
            ('Test01c-Pwrrt-SetValues.Float32', 22.22, ''),
            ('Test01c-Pwrrt-SetValues.Float64', 4444.4444, ''),
            ('Test01c-Pwrrt-SetValues.String80', "Why don't you have wings to fly with?", ''),
            ('Test01c-Pwrrt-SetValues.Time', '15-MAY-2020 08:27:50.50', ''),
            ('Test01c-Pwrrt-SetValues.DeltaTime', '27 8:27:50.05', ''),
            ('Test01c-Pwrrt-SetValues.Objid', pwrrt.Oid('VolPwrTest01c:Test01c-Pwrrt-SetValues'), ''),
            ('Test01c-Pwrrt-SetValues.AttrRef', pwrrt.Aref('VolPwrTest01c:Test01c-Pwrrt-SetValues.AttrRef'), ''),
            ('Test01c-Pwrrt-SetValues.BooleanArray[4]', 1, ''),
            ('Test01c-Pwrrt-SetValues.Int8Array[4]', 22, ''),
            ('Test01c-Pwrrt-SetValues.Int16Array[4]', 333, ''),
            ('Test01c-Pwrrt-SetValues.Int32Array[4]', 4444, ''),
            ('Test01c-Pwrrt-SetValues.Int64Array[4]', 55555, ''),
            ('Test01c-Pwrrt-SetValues.UInt8Array[4]', 66, ''),
            ('Test01c-Pwrrt-SetValues.UInt16Array[4]', 777, ''),
            ('Test01c-Pwrrt-SetValues.UInt32Array[4]', 8888, ''),
            ('Test01c-Pwrrt-SetValues.UInt64Array[4]', 99999, ''),
            ('Test01c-Pwrrt-SetValues.Float32Array[4]', 22.22, ''),
            ('Test01c-Pwrrt-SetValues.Float64Array[4]', 4444.4444, ''),
            ('Test01c-Pwrrt-SetValues.String80Array[4]', "Why don't you have wings to fly with?", ''),
            ('Test01c-Pwrrt-SetValues.TimeArray[4]', '15-MAY-2020 08:27:50.50', ''),
            ('Test01c-Pwrrt-SetValues.DeltaTimeArray[4]', '27 8:27:50.05', ''),
            ('Test01c-Pwrrt-SetValues.ObjidArray[4]', pwrrt.Oid('VolPwrTest01c:Test01c-Pwrrt-SetValues'), ''),
            ('Test01c-Pwrrt-SetValues.AttrRefArray[4]', pwrrt.Aref('VolPwrTest01c:Test01c-Pwrrt-SetValues.AttrRefArray[4]'), '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                a.setValue(d[i][1])
                value = a.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "Aref.setValue, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                elif type(value).__name__ == 'Oid' or type(value).__name__ == 'Aref' :
                    if value.fullName() != d[i][1].fullName():
                        self.logger.vlog('E', "Aref.setValue, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value.fullName(), d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "Aref.setValue, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Aref.setValue, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref.setValue, successfull completion')

    #
    # Aref.subscribe()
    #
    def Aref_subscribe(self):
        d = (
            ('Test01c-Pwrrt-Av1.ActualValue', 22.22, ''),
            ('Test01c-Pwrrt-Av1.NoSuchAttr', 22.22, '%GDH-E-ATTRIBUTE, no such attribute'),
            ('Test01c-Pwrrt-Dv1.ActualValue', 0, ''),
            ('Test01c-Pwrrt-Dv2.ActualValue', 1, ''),
            ('Test01c-Pwrrt-L1.Value.ActualValue', 33.33, ''),
            ('Test01c-Pwrrt-L1.LimitHH.Limit', 95, ''),
            ('Test01c-Pwrrt-L1.Description', 'BaseLevelSensor used for python tests', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0]);
                sub = a.subscribe()
                value = sub.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "Aref.subscribe, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "Aref.subscribe, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
                sub.close();
            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Aref.subscribe, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Aref.subscribe, successfull completion')


    #
    # Cid functions
    #

    #
    # Cid()
    #
    def Cid(self):
        d = (
            ('Av', ''),
            ('$PlantHier', ''),
            ('$ClassDef', ''),
            ('$Attribute', ''),
            ('$ClassVolume', ''),
            ('Aggregate', ''),
            ('BaseLevelSensor', ''),
            ('Pwrt01_Values', ''),
            ('Pwrt01_Pump', ''),
            ('No', '%GDH-E-BADOBJTYPE, bad object type number specified')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrrt.Cid(d[i][0])
                name = c.name()
                if name != d[i][0]:
                    self.logger.vlog('E', "Cid, Name doesn't match, %s != %s, idx %s",
                                    name, d[i][0], str(i))
                    return

            except RuntimeError as e:
                if str(e) != d[i][1]:
                    self.logger.vlog('E', 'Cid, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
                
        self.logger.log('S', 'Cid, successfull completion')


    #
    # Cid.fullName()
    #
    def Cid_fullName(self):
        d = (
            ('Av', 'pwrb:Class-Av', ""),
            ('$PlantHier', 'pwrs:Class-$PlantHier', ''),
            ('$ClassDef', 'pwrs:Class-$ClassDef', ''),
            ('$Attribute', 'pwrs:Class-$Attribute', ''),
            ('$ClassVolume', 'pwrs:Class-$ClassVolume', ''),
            ('Aggregate', 'BaseComponent:Class-Aggregate', ''),
            ('BaseLevelSensor', 'BaseComponent:Class-BaseLevelSensor', ''),
            ('Pwrt01_Values', 'CVolPwrtest01:Class-Pwrt01_Values', ''),
            ('Pwrt01_Pump', 'CVolPwrtest01:Class-Pwrt01_Pump', ''),
            ('No', '', '%GDH-E-BADOBJTYPE, bad object type number specified')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrrt.Cid(d[i][0])
                name = c.fullName()
                if name != d[i][1]:
                    self.logger.vlog('E', "Cid.fullName, Name doesn't match, %s != %s",
                                     name, d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Cid.fullName, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Cid.fullName, successfull completion')
    
    #
    # Cid.object()
    #
    def Cid_object(self):
        d = (
            ('MessageHandler', 'VolPwrTest01c:Nodes-PwrTest01c-Servers-MessageHandler'),
            ('IOHandler', 'VolPwrTest01c:Nodes-PwrTest01c-Servers-IOHandler'),
            ('$PlantHier', 'VolPwrTest01c:RootObject'),
            ('AArray100', 'VolPwrTest01c:RootObject-Child1-A1'),
            ('BaseFcPPO3PumpAggr', 'VolPwrTest01c:RootObject-Child1-P1'),
            ('$WorkBenchVolume', 'None')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrrt.Cid(d[i][0])
                o = c.object()
                if o is None:
                    if d[i][1] != 'None':
                        self.logger.vlog('E', "Cid.object, No instance should be found, %s",
                                         d[i][0])
                        return
                else:
                    name = o.fullName()
                    if name != d[i][1]:
                        self.logger.vlog('E', "Cid.object, Name doesn't match, %s != %s",
                                         name, d[i][1])
                        return


            except RuntimeError as e:
                self.logger.vlog('E', 'Cid.object, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Cid.object, successfull completion')
    

    #
    # Cid.nextObject()
    #
    def Cid_nextObject(self):
        d = (
            ('VolPwrTest01c:Nodes-PwrTest01c-Servers-MessageHandler', 'None'),
            ('VolPwrTest01c:Nodes-PwrTest01c-Servers-IOHandler', 'None'),
            ('VolPwrTest01c:RootObject', 'VolPwrTest01c:RootObject-Child1'),
            ('VolPwrTest01c:RootObject-Child1', 'VolPwrTest01c:RootObject-Child2'),
            ('VolPwrTest01c:RootObject-Child2', 'VolPwrTest01c:RootObject-Child3'),
            ('VolPwrTest01c:RootObject-Child3', 'VolPwrTest01c:RootObject-Child4'),
            ('VolPwrTest01c:RootObject-Child4', 'VolPwrTest01c:RootObject-ChildLess')
            )
        i = 0
        for i in range(len(d)):
            try:
                o = pwrrt.Oid(d[i][0])
                c = o.cid()
                n = c.nextObject(o)
                if n is None:
                    if d[i][1] != 'None':
                        self.logger.vlog('E', "Cid.nextObject, No instance should be found, %s",
                                         d[i][0])
                        return
                else:
                    name = n.fullName()
                    if name != d[i][1]:
                        self.logger.vlog('E', "Cid.nextObject, Name doesn't match, %s != %s",
                                         name, d[i][1])
                        return


            except RuntimeError as e:
                self.logger.vlog('E', 'Cid.nextObject, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Cid.nextObject, successfull completion')
    
    #
    # Cid.objects()
    #
    def Cid_objects(self):
        d = (
            ('$PlantHier', ('RootObject', 'Child1', 'Child2', 'Child3', 'Child4', 'ChildLess')),
            ('AArray100', ('A1', 'A2'))
            )

        i = 0
        for i in range(len(d)):
            j = 0
            try:                
                c = pwrrt.Cid(d[i][0])
                objectlist = c.objects()
                for o in objectlist:
                    if j >= len(d[i][1]):
                        break
                    if o.name() != d[i][1][j]:
                        self.logger.vlog('E', "Cid.objects, Name doesn't match, %s != %s",
                                         o.fullName(), d[i][1][j])
                        return
                    j += 1
            except RuntimeError as e:
                self.logger.vlog('E', 'Cid.objects, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return

        self.logger.log('S', 'Cid.objects, successfull completion')


    #
    # Cid.attrObject()
    #
    def Cid_attrObject(self):
        d = (
            ('$Node', 'VolPwrTest01c:Nodes-PwrTest01c'),
            ('$Security', 'VolPwrTest01c:Nodes-PwrTest01c-Security'),
            ('OpPlace', 'VolPwrTest01c:Nodes-PwrTest01c-OpPlaces-Op'),
            ('PlcPgm', 'VolPwrTest01c:Test01c-First-Plc'),
            ('And', 'VolPwrTest01c:Test01c-First-Plc-W-And0'),
            ('Dv', 'VolPwrTest01c:Test01c-First-Dv1'),
            ('Pwrt01_Roller', 'VolPwrTest01c:Test01c-First-R1'),
            ('Pwrt01_Motor', 'VolPwrTest01c:Test01c-First-M1'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-Comp1'),
            ('Pwrt01_Contactor', 'VolPwrTest01c:Test01c-First-C1')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrrt.Cid(d[i][0])
                o = c.attrObject()
                if o is None:
                    if d[i][1] != 'None':
                        self.logger.vlog('E', "Cid.attrObject, No instance should be found, %s",
                                         d[i][0])
                        return
                else:
                    name = o.fullName()
                    if name != d[i][1]:
                        self.logger.vlog('E', "Cid.attrObject, Name doesn't match, %s != %s",
                                         name, d[i][1])
                        return


            except RuntimeError as e:
                self.logger.vlog('E', 'Cid.attrObject, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Cid.attrObject, successfull completion')
    


    #
    # Cid.nextAttrObject()
    #
    def Cid_nextAttrObject(self):
        d = (
            ('MessageHandler', 'VolPwrTest01c:Nodes-PwrTest01c-Servers-MessageHandler', 'None'),
            ('IOHandler', 'VolPwrTest01c:Nodes-PwrTest01c-Servers-IOHandler', 'None'),
            ('$PlantHier', 'VolPwrTest01c:RootObject', 'VolPwrTest01c:RootObject-Child1'),
            ('$PlantHier', 'VolPwrTest01c:RootObject-Child1', 'VolPwrTest01c:RootObject-Child2'),
            ('$PlantHier', 'VolPwrTest01c:RootObject-Child2', 'VolPwrTest01c:RootObject-Child3'),
            ('$PlantHier', 'VolPwrTest01c:RootObject-Child3', 'VolPwrTest01c:RootObject-Child4'),
            ('$PlantHier', 'VolPwrTest01c:RootObject-Child4', 'VolPwrTest01c:RootObject-ChildLess'),
            ('Dv', 'VolPwrTest01c:Test01c-First-Dv1', 'VolPwrTest01c:Test01c-First-Dv2'),
            ('Dv', 'VolPwrTest01c:Test01c-First-Dv2', 'VolPwrTest01c:Test01c-First-Dv3'),
            ('Dv', 'VolPwrTest01c:Test01c-First-Dv3', 'VolPwrTest01c:Test01c-First-Dv4'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-Comp1', 'VolPwrTest01c:Test01c-First-C1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-C1.Super', 'VolPwrTest01c:Test01c-First-M1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-M1.Super', 'VolPwrTest01c:Test01c-First-M1.Contactor1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-M1.Contactor1.Super', 'VolPwrTest01c:Test01c-First-R1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[0].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[0].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[0].Contactor2.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[0].Contactor2.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[1].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[1].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[1].Contactor1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[1].Contactor1.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[2].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[2].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[2].Contactor2.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[2].Contactor2.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[3].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[3].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[3].Contactor1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[3].Contactor1.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[4].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[4].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[4].Contactor2.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[4].Contactor2.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[5].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[5].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[5].Contactor1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[5].Contactor1.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[6].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[6].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[6].Contactor2.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[6].Contactor2.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[7].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[7].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[7].Contactor1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[7].Contactor1.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[8].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[8].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[8].Contactor1.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[8].Contactor1.Super', 'VolPwrTest01c:Test01c-First-R1.Motors[9].Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[9].Super', 'VolPwrTest01c:Test01c-First-R1.Motors[9].Contactor2.Super'),
            ('Pwrt01_Component', 'VolPwrTest01c:Test01c-First-R1.Motors[9].Contactor2.Super', 'None')
            )
        i = 0
        for i in range(len(d)):
            try:
                a = pwrrt.Aref(d[i][1])
                c = pwrrt.Cid(d[i][0])
                n = c.nextAttrObject(a)
                if n is None:
                    if d[i][2] != 'None':
                        self.logger.vlog('E', "Cid.nextAttrObject, No instance should be found, %s",
                                         d[i][1])
                        return
                else:
                    name = n.fullName()
                    if name != d[i][2]:
                        self.logger.vlog('E', "Cid.nextAttrObject, Name doesn't match, %s != %s",
                                         name, d[i][2])
                        return


            except RuntimeError as e:
                self.logger.vlog('E', 'Cid.nextAttrObject, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Cid.nextAttrObject, successfull completion')
    

    #
    # Cid.attrObjects()
    #
    def Cid_attrObjects(self):
        d = (
            ('$PlantHier', ('RootObject', 'Child1', 'Child2', 'Child3', 'Child4', 'ChildLess')),
            ('AArray100', ('A1', 'A2')),
            ('Pwrt01_Component',(
             'Comp1',
             'C1.Super',
             'M1.Super',
             'M1.Contactor1.Super',
             'R1.Super',
             'R1.Motors[0].Super',
             'R1.Motors[0].Contactor2.Super',
             'R1.Motors[1].Super',
             'R1.Motors[1].Contactor1.Super',
             'R1.Motors[2].Super',
             'R1.Motors[2].Contactor2.Super',
             'R1.Motors[3].Super',
             'R1.Motors[3].Contactor1.Super',
             'R1.Motors[4].Super',
             'R1.Motors[4].Contactor2.Super',
             'R1.Motors[5].Super',
             'R1.Motors[5].Contactor1.Super',
             'R1.Motors[6].Super',
             'R1.Motors[6].Contactor2.Super',
             'R1.Motors[7].Super',
             'R1.Motors[7].Contactor1.Super',
             'R1.Motors[8].Super',
             'R1.Motors[8].Contactor1.Super',
             'R1.Motors[9].Super',
             'R1.Motors[9].Contactor2.Super'))
            )

        i = 0
        for i in range(len(d)):
            j = 0
            try:                
                c = pwrrt.Cid(d[i][0])
                objectlist = c.attrObjects()
                for a in objectlist:
                    if j >= len(d[i][1]):
                        break
                    if a.name() != d[i][1][j]:
                        self.logger.vlog('E', "Cid.attrObjects, Name doesn't match, %s != %s",
                                         a.fullName(), d[i][1][j])
                        return
                    j += 1
            except RuntimeError as e:
                self.logger.vlog('E', 'Cid.attrObjects, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return

        self.logger.log('S', 'Cid.attrObjects, successfull completion')

    #
    # Cid.attributes()
    #
    def Cid_attributes(self):
        d = (
            ('$PlantHier', ('Description', 'DefGraph', 'DefTrend', 'HelpTopic',
                            'DataSheet', 'CircuitDiagram', 'Photo',
                            'ConfigurationStatus', 'SafetyLevel')),
            ('And' ,('In1', 'In2', 'In3', 'In4', 'In5', 'In6', 'In7', 'In8', 'Status'))
            )

        i = 0
        for i in range(len(d)):
            j = 0
            try:                
                c = pwrrt.Cid(d[i][0])
                alist = c.attributes()
                for a in alist:
                    if j >= len(d[i][1]):
                        break
                    if a.name() != d[i][1][j]:
                        self.logger.vlog('E', "Cid.attributes, Name doesn't match, %s != %s",
                                         a.name(), d[i][1][j])
                        return
                    j += 1
            except RuntimeError as e:
                self.logger.vlog('E', 'Cid.attributes, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return

        self.logger.log('S', 'Cid.attributes, successfull completion')

    #
    # Tid.name()
    #
    def Tid_name(self):
        d = (
            ('RootObject.Description', '$String80', ''),
            ('RootObject-Child1.DefGraph', '$AttrRef', ''),
            ('Test01c-Pwrrt-Dv1.InitialValue', '$Boolean', ''),
            ('Test01c-Pwrrt-L1.Value', 'Ai', ''),
            ('Test01c-Pwrrt-L1.Value.ActualValue', '$Float32', ''),
            ('Test01c-Pwrrt-Values.Boolean', '$Boolean', ''),
            ('Test01c-Pwrrt-Values.Int8', '$Int8', ''),
            ('Test01c-Pwrrt-Values.Int16', '$Int16', ''),
            ('Test01c-Pwrrt-Values.Int32', '$Int32', ''),
            ('Test01c-Pwrrt-Values.Int64', '$Int64', ''),
            ('Test01c-Pwrrt-Values.UInt8', '$UInt8', ''),
            ('Test01c-Pwrrt-Values.UInt16', '$UInt16', ''),
            ('Test01c-Pwrrt-Values.UInt32', '$UInt32', ''),
            ('Test01c-Pwrrt-Values.UInt64', '$UInt64', ''),
            ('Test01c-Pwrrt-Values.Float32', '$Float32', ''),
            ('Test01c-Pwrrt-Values.Float64', '$Float64', ''),
            ('Test01c-Pwrrt-Values.String80', '$String80', ''),
            ('Test01c-Pwrrt-Values.Time', '$Time', ''),
            ('Test01c-Pwrrt-Values.DeltaTime', '$DeltaTime', ''),
            ('Test01c-Pwrrt-Values.Objid', '$Objid', ''),
            ('Test01c-Pwrrt-Values.AttrRef', '$AttrRef', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrrt.attribute(d[i][0])
                tid = a.tid()
                if tid.name() != d[i][1]:
                    self.logger.vlog('E', "Tid.name, "
                                     "Name doesn't match, %s != %s",
                                     tid.name(), d[i][1])
                    return


            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Tid.name, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'Tid.name, successfull completion')
    
    #
    # ADef.name()
    #
    def ADef_name(self):
        d = (
            ('And',
             (('In1', 8, 4, 1, 1104), # name, offset, size, elememts, flags
              ('In2', 24, 4, 1, 1104),
              ('In3', 40, 4, 1, 1104),
              ('In4', 56, 4, 1, 1104),
              ('In5', 72, 4, 1, 1104),
              ('In6', 88, 4, 1, 1104),
              ('In7', 104, 4, 1, 1104),
              ('In8', 120, 4, 1, 1104),
              ('Status', 124, 4, 1, 1044))),
            ('Pwrt01_Values',
             (('Boolean', 0, 4, 1, 0),
              ('Int8', 4, 1, 1, 0),
              ('Int16', 8, 2, 1, 0),
              ('Int32', 12, 4, 1, 0),
              ('Int64', 16, 8, 1, 0),
              ('UInt8', 24, 1, 1, 0),
              ('UInt16', 28, 2, 1, 0),
              ('UInt32', 32, 4, 1, 0),
              ('UInt64', 40, 8, 1, 0),
              ('Float32', 48, 4, 1, 0),
              ('Float64', 56, 8, 1, 0),
              ('String80', 64, 80, 1, 0),
              ('ProString40', 144, 40, 1, 0),
              ('Time', 184, 16, 1, 0),
              ('DeltaTime', 200, 16, 1, 0),
              ('Objid', 216, 8, 1, 0),
              ('AttrRef', 224, 24, 1, 0),
              ('BooleanArray', 248, 20, 5, 2),
              ('Int8Array', 268, 5, 5, 2),
              ('Int16Array', 276, 10, 5, 2),
              ('Int32Array', 288, 20, 5, 2),
              ('Int64Array', 312, 40, 5, 2),
              ('UInt8Array', 352, 5, 5, 2),
              ('UInt16Array', 360, 10, 5, 2),
              ('UInt32Array', 372, 20, 5, 2),
              ('UInt64Array', 392, 40, 5, 2),
              ('Float32Array', 432, 20, 5, 2),
              ('Float64Array', 456, 40, 5, 2),
              ('String80Array', 496, 400, 5, 2),
              ('TimeArray', 896, 80, 5, 2),
              ('DeltaTimeArray', 976, 80, 5, 2),
              ('ObjidArray', 1056, 40, 5, 2),
              ('AttrRefArray', 1096, 120, 5, 2)))
            )
        i = 0
        for i in range(len(d)):
            try:                
                cid = pwrrt.Cid(d[i][0])
                adeflist = cid.attributes()
                j = 0
                for adef in adeflist:
                    if adef.name() != d[i][1][j][0]:
                        self.logger.vlog('E', "ADef.name, "
                                     "name doesn't match, %s != %s",
                                         adef.name(), d[i][1][j][0])
                        return
                    if adef.offset() != d[i][1][j][1]:
                        self.logger.vlog('E', "ADef.name, "
                                     "offset doesn't match, %s != %s",
                                         adef.name(), str(d[i][1][j][1]))
                        return
                    if adef.size() != d[i][1][j][2]:
                        self.logger.vlog('E', "ADef.name, "
                                     "size doesn't match, %s != %s",
                                         adef.name(), str(d[i][1][j][2]))
                        return
                    if adef.elements() != d[i][1][j][3]:
                        self.logger.vlog('E', "ADef.name, "
                                     "elements doesn't match, %s != %s",
                                         adef.name(), str(d[i][1][j][3]))
                        return
                    if adef.flags() != d[i][1][j][4]:
                        self.logger.vlog('E', "ADef.name, "
                                     "flags doesn't match, %s != %s",
                                         adef.name(), str(d[i][1][j][4]))
                        return
                    j += 1

            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'ADef.name, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
        self.logger.log('S', 'ADef.name, successfull completion')
    

    #
    # Sub.setValue()
    #
    #
    # Sub.setValue()
    #
    def Sub_setValue(self):
        d = (
            ('Test01c-Pwrrt-SetValues.Boolean', 1, ''),
            ('Test01c-Pwrrt-SetValues.Int8', 11, ''),
            ('Test01c-Pwrrt-SetValues.Int16', 222, ''),
            ('Test01c-Pwrrt-SetValues.Int32', 3333, ''),
            ('Test01c-Pwrrt-SetValues.Int64', 44444, ''),
            ('Test01c-Pwrrt-SetValues.UInt8', 55, ''),
            ('Test01c-Pwrrt-SetValues.UInt16', 666, ''),
            ('Test01c-Pwrrt-SetValues.UInt32', 7777, ''),
            ('Test01c-Pwrrt-SetValues.UInt64', 88888, ''),
            ('Test01c-Pwrrt-SetValues.Float32', 11.111, ''),
            ('Test01c-Pwrrt-SetValues.Float64', 3333.3333, ''),
            ('Test01c-Pwrrt-SetValues.String80', "Why don't you have wings to fly with?", ''),
            ('Test01c-Pwrrt-SetValues.Time', '19-MAY-2020 07:53:34.50', ''),
            ('Test01c-Pwrrt-SetValues.DeltaTime', '27 7:53:34.05', ''),
            ('Test01c-Pwrrt-SetValues.Objid', pwrrt.Oid('VolPwrTest01c:Test01c-Pwrrt-SetValues'), ''),
            ('Test01c-Pwrrt-SetValues.AttrRef', pwrrt.Aref('VolPwrTest01c:Test01c-Pwrrt-SetValues.AttrRef'), ''),
            ('Test01c-Pwrrt-SetValues.BooleanArray[4]', 0, ''),
            ('Test01c-Pwrrt-SetValues.Int8Array[4]', 33, ''),
            ('Test01c-Pwrrt-SetValues.Int16Array[4]', 444, ''),
            ('Test01c-Pwrrt-SetValues.Int32Array[4]', 5555, ''),
            ('Test01c-Pwrrt-SetValues.Int64Array[4]', 66666, ''),
            ('Test01c-Pwrrt-SetValues.UInt8Array[4]', 77, ''),
            ('Test01c-Pwrrt-SetValues.UInt16Array[4]', 888, ''),
            ('Test01c-Pwrrt-SetValues.UInt32Array[4]', 9999, ''),
            ('Test01c-Pwrrt-SetValues.UInt64Array[4]', 111111, ''),
            ('Test01c-Pwrrt-SetValues.Float32Array[4]', 33.33, ''),
            ('Test01c-Pwrrt-SetValues.Float64Array[4]', 5555.5555, ''),
            ('Test01c-Pwrrt-SetValues.String80Array[4]', "Why don't you have wings to fly with?", ''),
            ('Test01c-Pwrrt-SetValues.TimeArray[4]', '19-MAY-2020 08:53:34.50', ''),
            ('Test01c-Pwrrt-SetValues.DeltaTimeArray[4]', '27 8:53:34.05', ''),
            ('Test01c-Pwrrt-SetValues.ObjidArray[4]', pwrrt.Oid('VolPwrTest01c:Test01c-Pwrrt-SetValues'), ''),
            ('Test01c-Pwrrt-SetValues.AttrRefArray[4]', pwrrt.Aref('VolPwrTest01c:Test01c-Pwrrt-SetValues.AttrRefArray[4]'), '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                sub = pwrrt.subscribe(d[i][0])
                sub.setValue(d[i][1])
                value = sub.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "Sub.setValue, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                elif type(value).__name__ == 'Oid' or type(value).__name__ == 'Aref' :
                    if value.fullName() != d[i][1].fullName():
                        self.logger.vlog('E', "Sub.setValue, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    value.fullName(), d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "Sub.setValue, "
                                         "Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
                sub.close();
            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Sub.setValue, '
                                     'Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    return
            except SystemError as e:
                self.logger.vlog('E', 'Sub.setValue, '
                                 'System exception %s, idx %s',
                                 str(e), str(i))
                return
        self.logger.log('S', 'Sub.setValue, successfull completion')



ctx = Ctx()
ctx.logger = pwrtest.logger('rt-pwrrt', '$pwrp_log/pwrrt.tlog')
try:
    ctx.init()
except:
    quit()
ctx.login()
pwrrt.login('pwrp', 'pwrp')
ctx.volume()
ctx.volumes()
ctx.object()
ctx.attribute()
ctx.subscribe()
ctx.Oid()
ctx.Oid_child()
ctx.Oid_next()
ctx.Oid_parent()
ctx.Oid_children()
ctx.Oid_name()
ctx.Oid_fullName()
ctx.Oid_oidStr()
ctx.Oid_cid()
ctx.Oid_attribute()
ctx.Aref()
ctx.Aref_name()
ctx.Aref_fullName()
ctx.Aref_arefStr()
ctx.Aref_tid()
ctx.Aref_value()
ctx.Aref_setValue()
ctx.Aref_subscribe()
ctx.Cid()
ctx.Cid_fullName()
ctx.Cid_object()
ctx.Cid_nextObject()
ctx.Cid_objects()
ctx.Cid_attrObject()
ctx.Cid_nextAttrObject()
ctx.Cid_attrObjects()
ctx.Cid_attributes()
ctx.Tid_name()
ctx.ADef_name()
ctx.Sub_setValue()
