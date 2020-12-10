#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import math
import pwrwb
import pwrtest

class Ctx:

    #
    # Static pwrrt methods
    #

    #
    # pwrwb.init()
    #
    def init(self):
        try:                
            pwrwb.open('VolPwrtest02')
        except RuntimeError as e:
            self.logger.vlog('E', 'init, Unexpected exception %s', str(e))
            raise e
        self.logger.log('S', 'init, successfull completion')

    #
    # pwrwb.login()/logout()
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
                pwrwb.login(d[i][0], d[i][1])
                user = pwrwb.getUser()
                priv = pwrwb.getPriv()
                if user != d[i][0]:
                    self.logger.vlog('E', 'login, getUser, wrong user %s, %s',
                                     user, d[i][1])
                    pwrwb.logout()
                    return
                if priv != d[i][2]:
                    self.logger.vlog('E', 'login, getPriv, wrong priv %s, %s',
                                     str(priv), str(d[i][2]))
                    pwrwb.logout()
                    return
                pwrwb.logout()
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
            ('VolPwrtest02', ''),
            ('CVolPwrtest02', ''),
            ('1_254_254_203', '%LDH-E-NOSUCHVOL, no such volume exists'),
            ('rt', ''),
            ('VolNoVolume', '%LDH-E-NOSUCHVOL, no such volume exists'),
            )
        i = 0
        for i in range(len(d)):
            try:                
                vol = pwrwb.volume(d[i][0])
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
    # pwrwb.volumes()
    #
    def volumes(self):
        d = ('VolPwrtest02', 'pwrs', 'pwrb', 'Simul', 'BaseComponent',
             'NMps', 'Profibus', 'CVolPwrtest02')
        vol = pwrwb.volumes()
        try:
            i = 0
            for dv in d:
                found = False
                for v in vol:
                    if v.name() == dv:
                        found = True
                if not found:
                    self.logger.vlog('E', "volumes, Volume not found, %s",
                                    v.name())
                    return
                i += 1
        except RuntimeError as e:
            self.logger.vlog('E', 'volumes, Unexpected exception %s, idx %s',
                                     str(e), str(i))
            return
        self.logger.log('S', 'volumes, successfull completion')

    #
    # pwrwb.object()
    #
    def object(self):
        d = (
            ('Test02-Pwrwb-Av1', 'VolPwrtest02:Test02-Pwrwb-Av1', ''),
            ('VolPwrTest02:Test02-Pwrwb-Av1', 'VolPwrtest02:Test02-Pwrwb-Av1', ''),
            ('Test02-Pwrwb-Dv1', 'VolPwrtest02:Test02-Pwrwb-Dv1', ''),
            ('Test02-Pwrwb-Dv2', 'VolPwrtest02:Test02-Pwrwb-Dv2', ''),
            ('Test02-Pwrwb-L1', 'VolPwrtest02:Test02-Pwrwb-L1', ''),
            ('Test02-Pwrwb', 'VolPwrtest02:Test02-Pwrwb', ''),
            ('Test02', 'VolPwrtest02:Test02', ''),
            ('Test02-Pwrwb-NoObject', '', '%LDH-E-NOSUCHOBJ, no such object exists'),
            ('VolPwrtest02:', 'VolPwrtest02:', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrwb.object(d[i][0])
                name = o.fullName()
                if name != d[i][1]:
                    self.logger.vlog('E', "object, Name doesn't match, %s != %s, idx %s",
                                    name, d[i][1], str(i))
                    return

            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'object, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    print('Not correct error code')
                    return
                
        self.logger.log('S', 'object, successfull completion')

    #
    # pwrwb.attribute()
    #
    def attribute(self):
        d = (
            ('Test02-Pwrwb-Av1.InitialValue', 22.22, ''),
            ('Test02-Pwrwb-Av1.NoSuchAttr', 22.22, '%LDH-E-NOSUCHATTR, no such attribute exists'),
            ('Test02-Pwrwb-Dv1.InitialValue', 0, ''),
            ('Test02-Pwrwb-Dv2.InitialValue', 1, ''),
            ('Test02-Pwrwb-L1.Value.InitialValue', 33.33, ''),
            ('Test02-Pwrwb-L1.LimitHH.Limit', 94, ''),
            ('Test02-Pwrwb-L1.Description', 'BaseLevelSensor used for python tests', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
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
    # pwrwb.getSessionVolume()
    #
    def getSessionVolume(self):
        try:                
            v = pwrwb.getSessionVolume()
            if v.name() != 'VolPwrtest02':
                self.logger.vlog('E', "getSessionVolume, Volume doesn't match, %s",
                                    v.name())
        except RuntimeError as e:
            self.logger.vlog('E', 'getSessionVolume, Unexpected exception %s', str(e))
            return
        self.logger.log('S', 'getSessionVolume, successfull completion')


    #
    # pwrwb.getUser()
    #
    def getUser(self):
        try:                
            user = pwrwb.getUser()
            if user != 'pwrp':
                self.logger.vlog('E', "getUser, user doesn't match, %s", user)
        except RuntimeError as e:
            self.logger.vlog('E', 'getUser, Unexpected exception %s', str(e))
            return
        self.logger.log('S', 'getUser, successfull completion')

    #
    # pwrwb.sessionIsEmpty()
    #
    def sessionIsEmpty(self):
        try:                
            empty = pwrwb.sessionIsEmpty()
            if empty != 1:
                self.logger.vlog('E', "sessionIsEmpty, wrong value %d", empty)
        except RuntimeError as e:
            self.logger.vlog('E', 'sessionIsEmpty, Unexpected exception %s', str(e))
            return
        self.logger.log('S', 'sessionIsEmpty, successfull completion')


    #
    # pwrwb.revertSession()
    #
    def revertSession(self):
        d = (
            ('Test02-Pwrwb-Av1.InitialValue', 22.22, 44.44),
            ('Test02-Pwrwb-Dv1.InitialValue', 0, 1),
            ('Test02-Pwrwb-Dv2.InitialValue', 1, 0),
            ('Test02-Pwrwb-L1.Value.InitialValue', 33.33, 66.66),
            ('Test02-Pwrwb-L1.LimitHH.Limit', 94, 102),
            ('Test02-Pwrwb-L1.Description', 'BaseLevelSensor used for python tests', 'Some level sensor...')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
                a.setValue(d[i][2])
            except RuntimeError as e:
                self.logger.vlog('E', 'revertSession, Unexpected exception %s, idx %s',
                                 str(e), str(i))
                return


        empty = pwrwb.sessionIsEmpty()
        if empty != 0:
            self.logger.vlog('E', "revertSession sessionIsEmpty, wrong value %d", empty)

        pwrwb.revertSession()

        empty = pwrwb.sessionIsEmpty()
        if empty != 1:
            self.logger.vlog('E', "revertSession sessionIsEmpty, wrong value %d", empty)

        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
                value = a.value()
                if type(value).__name__ == 'str':
                    if value != d[i][1]:
                        self.logger.vlog('E', "revertSession, Value doesn't match, %s != %s, idx %s",
                                    value, d[i][1], str(i))
                        return
                else:
                    if not abs(value - d[i][1]) < 0.001:
                	self.logger.vlog('E', "revertSession, Value doesn't match, %s != %s, idx %s",
                                    str(value), str(d[i][1]), str(i))
                        return
            except RuntimeError as e:
                self.logger.vlog('E', 'revertSession, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                return

        self.logger.log('S', 'revertSession, successfull completion')

    #
    # Oid functions
    #

    #
    # Oid()
    #
    def Oid(self):
        d = (
            ('Test02-Pwrwb-Av1', 'VolPwrtest02:Test02-Pwrwb-Av1', ''),
            ('VolPwrtest02:Test02-Pwrwb-Av1', 'VolPwrtest02:Test02-Pwrwb-Av1', ''),
            ('Test02-Pwrwb-Dv1', 'VolPwrtest02:Test02-Pwrwb-Dv1', ''),
            ('Test02-Pwrwb-Dv2', 'VolPwrtest02:Test02-Pwrwb-Dv2', ''),
            ('Test02-Pwrwb-L1', 'VolPwrtest02:Test02-Pwrwb-L1', ''),
            ('Test02-Pwrwb', 'VolPwrtest02:Test02-Pwrwb', ''),
            ('Test02', 'VolPwrtest02:Test02', ''),
            ('Test02-Pwrwb-NoObject', '', '%LDH-E-NOSUCHOBJ, no such object exists'),
            ('VolPwrtest02:', 'VolPwrtest02:', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrwb.Oid(d[i][0])
                name = o.fullName()
                if name != d[i][1]:
                    self.logger.vlog('E', "Oid, Name doesn't match, %s != %s, idx %s",
                                    name, d[i][1], str(i))
                    return

            except RuntimeError as e:
                if str(e) != d[i][2]:
                    self.logger.vlog('E', 'Oid, Unexpected exception %s, idx %s',
                                     str(e), str(i))
                    print('Not correct error code')
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
                o = pwrwb.object(d[i][0])
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
                o = pwrwb.object(d[i][0])
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
            ('NoSuchObject', '', '%LDH-E-NOSUCHOBJ, no such object exists')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrwb.object(d[i][0])
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
                o = pwrwb.object(d[i][0])
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
            ('Test02-Ldh-LongName90123456789012345678901', 'LongName90123456789012345678901', ''),
            ('Test02-Ldh-ÄÅÖäåö', 'ÄÅÖäåö', ''),
            ('Test02-Ldh-sdf*sdf', '', '%LDH-E-BADNAME, bad object name'),
            ('VolPwrtest02:', 'VolPwrtest02', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrwb.object(d[i][0])
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
            ('RootObject', 'VolPwrtest02:RootObject', ''),
            ('RootObject-Child1', 'VolPwrtest02:RootObject-Child1', ''),
            ('Test02-Ldh-LongName90123456789012345678901', 'VolPwrtest02:Test02-Ldh-LongName90123456789012345678901', ''),
            ('Test02-Ldh-ÄÅÖäåö', 'VolPwrtest02:Test02-Ldh-ÄÅÖäåö', ''),
            ('Test02-Ldh-sdf*sdf', '', '%LDH-E-BADNAME, bad object name'),
            ('VolPwrtest02:', 'VolPwrtest02:', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrwb.object(d[i][0])
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
            ('RootObject', '_O0.254.254.210:1', ''),
            ('RootObject-Child1', '_O0.254.254.210:2', ''),
            ('VolPwrTest02:', '_O0.254.254.210:0', ''),
            ('pwrb:Class-And', '_O0.0.0.2:2149842944', ''),
            ('pwrs:', '_O0.0.0.1:0', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrwb.object(d[i][0])
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
    # Oid.attribute()
    #
    def Oid_attribute(self):
        d = (
            ('RootObject', 'Description', 'RootObject.Description'),
            ('RootObject-Child1', 'Description', 'Child1.Description'),
            ('Test02-Pwrwb-Dv1', 'ActualValue', 'Dv1.ActualValue'),
            ('Test02-Pwrwb-L1', 'LimitLL', 'L1.LimitLL'),
            ('Test02-Pwrwb-L1', 'Value.ActualValue', 'L1.Value.ActualValue')
            )
        i = 0
        for i in range(len(d)):
            try:                
                o = pwrwb.object(d[i][0])
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
            ('Test02-Pwrwb-Av1.InitialValue', 22.22, ''),
            ('Test02-Pwrwb-Av1.NoSuchAttr', 22.22, '%LDH-E-NOSUCHATTR, no such attribute exists'),
            ('Test02-Pwrwb-Dv1.InitialValue', 0, ''),
            ('Test02-Pwrwb-Dv2.InitialValue', 1, ''),
            ('Test02-Pwrwb-L1.Value.InitialValue', 33.33, ''),
            ('Test02-Pwrwb-L1.LimitHH.Limit', 94, ''),
            ('Test02-Pwrwb-L1.Description', 'BaseLevelSensor used for python tests', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.Aref(d[i][0])
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
            ('Test02-Ldh-LongName90123456789012345678901.InitialValue', 'LongName90123456789012345678901.InitialValue', ''),
            ('Test02-Ldh-ÄÅÖäåö.ValueIndex', 'ÄÅÖäåö.ValueIndex', ''),
            ('Test02-Ldh-sdf*sdf', '', '%LDH-E-NOSUCHATTR, no such attribute exists')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
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
            ('RootObject.Description', 'VolPwrtest02:RootObject.Description', ''),
            ('RootObject-Child1.DefGraph', 'VolPwrtest02:RootObject-Child1.DefGraph', ''),
            ('Test02-Ldh-LongName90123456789012345678901.InitialValue', 'VolPwrtest02:Test02-Ldh-LongName90123456789012345678901.InitialValue', ''),
            ('Test02-Ldh-ÄÅÖäåö.ValueIndex', 'VolPwrtest02:Test02-Ldh-ÄÅÖäåö.ValueIndex', ''),
            ('Test02-Ldh-sdf*sdf', '', '%LDH-E-NOSUCHATTR, no such attribute exists')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
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
            ('RootObject', '_A0.254.254.210:1(_T0.1:0.17.1)[0.656]', ''),
            ('RootObject-Child1', '_A0.254.254.210:2(_T0.1:0.17.1)[0.656]', ''),
            ('VolPwrTest02:', '_A0.254.254.210:0(_T0.1:0.47.1)[0.160]', ''),
            ('pwrb:Class-And', '_A0.0.0.2:2149842944(_T0.1:0.1.1)[0.24]', ''),
            ('pwrs:', '_A0.0.0.1:0(_T0.1:0.52.1)[0.208]', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
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
            ('Test02-Pwrwb-Dv1.InitialValue', 'pwrs:Type-$Boolean', ''),
            ('Test02-Pwrwb-L1.Value', 'pwrb:Class-Ai', ''),
            ('Test02-Pwrwb-L1.Value.ActualValue', 'pwrs:Type-$Float32', ''),
            ('Test02-Pwrwb-Values.Boolean', 'pwrs:Type-$Boolean', ''),
            ('Test02-Pwrwb-Values.Int8', 'pwrs:Type-$Int8', ''),
            ('Test02-Pwrwb-Values.Int16', 'pwrs:Type-$Int16', ''),
            ('Test02-Pwrwb-Values.Int32', 'pwrs:Type-$Int32', ''),
            ('Test02-Pwrwb-Values.Int64', 'pwrs:Type-$Int64', ''),
            ('Test02-Pwrwb-Values.UInt8', 'pwrs:Type-$UInt8', ''),
            ('Test02-Pwrwb-Values.UInt16', 'pwrs:Type-$UInt16', ''),
            ('Test02-Pwrwb-Values.UInt32', 'pwrs:Type-$UInt32', ''),
            ('Test02-Pwrwb-Values.UInt64', 'pwrs:Type-$UInt64', ''),
            ('Test02-Pwrwb-Values.Float32', 'pwrs:Type-$Float32', ''),
            ('Test02-Pwrwb-Values.Float64', 'pwrs:Type-$Float64', ''),
            ('Test02-Pwrwb-Values.String80', 'pwrs:Type-$String80', ''),
            ('Test02-Pwrwb-Values.Time', 'pwrs:Type-$Time', ''),
            ('Test02-Pwrwb-Values.DeltaTime', 'pwrs:Type-$DeltaTime', ''),
            ('Test02-Pwrwb-Values.Objid', 'pwrs:Type-$Objid', ''),
            ('Test02-Pwrwb-Values.AttrRef', 'pwrs:Type-$AttrRef', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
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
            ('Test02-Pwrwb-Av1.InitialValue', 22.22, ''),
            ('Test02-Pwrwb-Av1.NoSuchAttr', 22.22, '%LDH-E-NOSUCHATTR, no such attribute exists'),
            ('Test02-Pwrwb-Dv1.InitialValue', 0, ''),
            ('Test02-Pwrwb-Dv2.InitialValue', 1, ''),
            ('Test02-Pwrwb-L1.Value.InitialValue', 33.33, ''),
            ('Test02-Pwrwb-L1.LimitHH.Limit', 94, ''),
            ('Test02-Pwrwb-L1.Description', 'BaseLevelSensor used for python tests', ''),
            ('Test02-Pwrwb-Values.Boolean', 1, ''),
            ('Test02-Pwrwb-Values.Int8', 22, ''),
            ('Test02-Pwrwb-Values.Int16', 333, ''),
            ('Test02-Pwrwb-Values.Int32', 4444, ''),
            ('Test02-Pwrwb-Values.Int64', 55555, ''),
            ('Test02-Pwrwb-Values.UInt8', 66, ''),
            ('Test02-Pwrwb-Values.UInt16', 777, ''),
            ('Test02-Pwrwb-Values.UInt32', 8888, ''),
            ('Test02-Pwrwb-Values.UInt64', 99999, ''),
            ('Test02-Pwrwb-Values.Float32', 22.22, ''),
            ('Test02-Pwrwb-Values.Float64', 4444.4444, ''),
            ('Test02-Pwrwb-Values.String80', "Why don't you have wings to fly with?", ''),
            ('Test02-Pwrwb-Values.Time', '15-MAY-2020 08:27:50.50', ''),
            ('Test02-Pwrwb-Values.DeltaTime', '27 8:27:50.05', ''),
            ('Test02-Pwrwb-Values.Objid', 'VolPwrtest02:Test02-Pwrwb-Values', ''),
            ('Test02-Pwrwb-Values.AttrRef', 'VolPwrtest02:Test02-Pwrwb-Values.AttrRef', ''),
            ('Test02-Pwrwb-Values.BooleanArray[4]', 1, ''),
            ('Test02-Pwrwb-Values.Int8Array[4]', 22, ''),
            ('Test02-Pwrwb-Values.Int16Array[4]', 333, ''),
            ('Test02-Pwrwb-Values.Int32Array[4]', 4444, ''),
            ('Test02-Pwrwb-Values.Int64Array[4]', 55555, ''),
            ('Test02-Pwrwb-Values.UInt8Array[4]', 66, ''),
            ('Test02-Pwrwb-Values.UInt16Array[4]', 777, ''),
            ('Test02-Pwrwb-Values.UInt32Array[4]', 8888, ''),
            ('Test02-Pwrwb-Values.UInt64Array[4]', 99999, ''),
            ('Test02-Pwrwb-Values.Float32Array[4]', 22.22, ''),
            ('Test02-Pwrwb-Values.Float64Array[4]', 4444.4444, ''),
            ('Test02-Pwrwb-Values.String80Array[4]', "Why don't you have wings to fly with?", ''),
            ('Test02-Pwrwb-Values.TimeArray[4]', '15-MAY-2020 08:27:50.50', ''),
            ('Test02-Pwrwb-Values.DeltaTimeArray[4]', '27 8:27:50.05', ''),
            ('Test02-Pwrwb-Values.ObjidArray[4]', 'VolPwrtest02:Test02-Pwrwb-Values', ''),
            ('Test02-Pwrwb-Values.AttrRefArray[4]', 'VolPwrtest02:Test02-Pwrwb-Values.AttrRefArray[4]', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
                value = a.value()
                if type(value).__name__ == 'NoneType':
                    self.logger.vlog('E', "Aref.value, None value returned, idx %s",
                                     str(i))
                    return
                elif type(value).__name__ == 'str':
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
            ('Test02-Pwrwb-SetValues.Boolean', 1, ''),
            ('Test02-Pwrwb-SetValues.Int8', 22, ''),
            ('Test02-Pwrwb-SetValues.Int16', 333, ''),
            ('Test02-Pwrwb-SetValues.Int32', 4444, ''),
            ('Test02-Pwrwb-SetValues.Int64', 55555, ''),
            ('Test02-Pwrwb-SetValues.UInt8', 66, ''),
            ('Test02-Pwrwb-SetValues.UInt16', 777, ''),
            ('Test02-Pwrwb-SetValues.UInt32', 8888, ''),
            ('Test02-Pwrwb-SetValues.UInt64', 99999, ''),
            ('Test02-Pwrwb-SetValues.Float32', 22.22, ''),
            ('Test02-Pwrwb-SetValues.Float64', 4444.4444, ''),
            ('Test02-Pwrwb-SetValues.String80', "Why don't you have wings to fly with?", ''),
            ('Test02-Pwrwb-SetValues.Time', '15-MAY-2020 08:27:50.50', ''),
            ('Test02-Pwrwb-SetValues.DeltaTime', '27 8:27:50.05', ''),
            ('Test02-Pwrwb-SetValues.Objid', pwrwb.Oid('VolPwrTest02:Test02-Pwrwb-SetValues'), ''),
            ('Test02-Pwrwb-SetValues.AttrRef', pwrwb.Aref('VolPwrTest02:Test02-Pwrwb-SetValues.AttrRef'), ''),
            ('Test02-Pwrwb-SetValues.BooleanArray[4]', 1, ''),
            ('Test02-Pwrwb-SetValues.Int8Array[4]', 22, ''),
            ('Test02-Pwrwb-SetValues.Int16Array[4]', 333, ''),
            ('Test02-Pwrwb-SetValues.Int32Array[4]', 4444, ''),
            ('Test02-Pwrwb-SetValues.Int64Array[4]', 55555, ''),
            ('Test02-Pwrwb-SetValues.UInt8Array[4]', 66, ''),
            ('Test02-Pwrwb-SetValues.UInt16Array[4]', 777, ''),
            ('Test02-Pwrwb-SetValues.UInt32Array[4]', 8888, ''),
            ('Test02-Pwrwb-SetValues.UInt64Array[4]', 99999, ''),
            ('Test02-Pwrwb-SetValues.Float32Array[4]', 22.22, ''),
            ('Test02-Pwrwb-SetValues.Float64Array[4]', 4444.4444, ''),
            ('Test02-Pwrwb-SetValues.String80Array[4]', "Why don't you have wings to fly with?", ''),
            ('Test02-Pwrwb-SetValues.TimeArray[4]', '15-MAY-2020 08:27:50.50', ''),
            ('Test02-Pwrwb-SetValues.DeltaTimeArray[4]', '27 8:27:50.05', ''),
            ('Test02-Pwrwb-SetValues.ObjidArray[4]', pwrwb.Oid('VolPwrTest02:Test02-Pwrwb-SetValues'), ''),
            ('Test02-Pwrwb-SetValues.AttrRefArray[4]', pwrwb.Aref('VolPwrTest02:Test02-Pwrwb-SetValues.AttrRefArray[4]'), '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
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
            ('Pwrt02_Values', ''),
            ('Pwrt02_Pump', '%LDH-E-NOCLASS, the class does not exist'),
            ('No', '%LDH-E-NOCLASS, the class does not exist')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrwb.Cid(d[i][0])
                name = c.name()
                if name != d[i][0]:
                    self.logger.vlog('E', "Cid, Name doesn't match, %s != %s, idx %s",
                                    name, d[i][0], str(i))
                    return

            except RuntimeError as e:
                if str(e) != d[i][1]:
                    print(str(e), d[i][1])
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
            ('Pwrt02_Values', 'CVolPwrtest02:Class-Pwrt02_Values', ''),
            ('Pwrt02_Pump', 'CVolPwrtest02:Class-Pwrt02_Pump', '%LDH-E-NOCLASS, the class does not exist'),
            ('No', '', '%LDH-E-NOCLASS, the class does not exist')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrwb.Cid(d[i][0])
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
            ('MessageHandler', 'VolPwrtest02:Nodes-Pwrtest02-Servers-MessageHandler'),
            ('IOHandler', 'VolPwrtest02:Nodes-Pwrtest02-Servers-IOHandler'),
            ('$PlantHier', 'VolPwrtest02:RootObject'),
            ('AArray100', 'VolPwrtest02:RootObject-Child1-A1'),
            ('BaseFcPPO3PumpAggr', 'VolPwrtest02:RootObject-Child1-P1'),
            ('$WorkBenchVolume', 'None')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrwb.Cid(d[i][0])
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
            ('VolPwrtest02:Nodes-Pwrtest02-Servers-MessageHandler', 'None'),
            ('VolPwrtest02:Nodes-Pwrtest02-Servers-IOHandler', 'None'),
            ('VolPwrtest02:RootObject', 'VolPwrtest02:RootObject-Child1'),
            ('VolPwrtest02:RootObject-Child1', 'VolPwrtest02:RootObject-Child2'),
            ('VolPwrtest02:RootObject-Child2', 'VolPwrtest02:RootObject-Child3'),
            ('VolPwrtest02:RootObject-Child3', 'VolPwrtest02:RootObject-Child4'),
            ('VolPwrtest02:RootObject-Child4', 'VolPwrtest02:RootObject-ChildLess')
            )
        i = 0
        for i in range(len(d)):
            try:
                o = pwrwb.Oid(d[i][0])
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
                c = pwrwb.Cid(d[i][0])
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
            ('$Node', 'VolPwrtest02:Nodes-Pwrtest02'),
            ('$Security', 'VolPwrtest02:Nodes-Pwrtest02-Security'),
            ('OpPlace', 'VolPwrtest02:Nodes-Pwrtest02-OpPlaces-Op'),
            ('PlcPgm', 'VolPwrtest02:PlcTest-H1-Plc'),
            ('And', 'VolPwrtest02:PlcTest-H1-Plc-W-And0'),
            ('Dv', 'VolPwrtest02:Test02-First-Dv1'),
            ('Pwrt02_Values', 'VolPwrtest02:H1-Values')
            )
        i = 0
        for i in range(len(d)):
            try:                
                c = pwrwb.Cid(d[i][0])
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
            ('MessageHandler', 'VolPwrtest02:Nodes-PwrTest02-Servers-MessageHandler', 'None'),
            ('IOHandler', 'VolPwrtest02:Nodes-PwrTest02-Servers-IOHandler', 'None'),
            ('$PlantHier', 'VolPwrtest02:RootObject', 'VolPwrtest02:RootObject-Child1'),
            ('$PlantHier', 'VolPwrtest02:RootObject-Child1', 'VolPwrtest02:RootObject-Child2'),
            ('$PlantHier', 'VolPwrtest02:RootObject-Child2', 'VolPwrtest02:RootObject-Child3'),
            ('$PlantHier', 'VolPwrtest02:RootObject-Child3', 'VolPwrtest02:RootObject-Child4'),
            ('$PlantHier', 'VolPwrtest02:RootObject-Child4', 'VolPwrtest02:RootObject-ChildLess'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-Comp1', 'VolPwrtest02:Test02-First-C1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-C1.Super', 'VolPwrtest02:Test02-First-M1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-M1.Super', 'VolPwrtest02:Test02-First-M1.Contactor1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-M1.Contactor1.Super', 'VolPwrtest02:Test02-First-R1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Super', 'VolPwrtest02:Test02-First-R1.Motors[0].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[0].Super', 'VolPwrtest02:Test02-First-R1.Motors[0].Contactor2.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[0].Contactor2.Super', 'VolPwrtest02:Test02-First-R1.Motors[1].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[1].Super', 'VolPwrtest02:Test02-First-R1.Motors[1].Contactor1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[1].Contactor1.Super', 'VolPwrtest02:Test02-First-R1.Motors[2].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[2].Super', 'VolPwrtest02:Test02-First-R1.Motors[2].Contactor2.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[2].Contactor2.Super', 'VolPwrtest02:Test02-First-R1.Motors[3].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[3].Super', 'VolPwrtest02:Test02-First-R1.Motors[3].Contactor1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[3].Contactor1.Super', 'VolPwrtest02:Test02-First-R1.Motors[4].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[4].Super', 'VolPwrtest02:Test02-First-R1.Motors[4].Contactor2.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[4].Contactor2.Super', 'VolPwrtest02:Test02-First-R1.Motors[5].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[5].Super', 'VolPwrtest02:Test02-First-R1.Motors[5].Contactor1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[5].Contactor1.Super', 'VolPwrtest02:Test02-First-R1.Motors[6].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[6].Super', 'VolPwrtest02:Test02-First-R1.Motors[6].Contactor2.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[6].Contactor2.Super', 'VolPwrtest02:Test02-First-R1.Motors[7].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[7].Super', 'VolPwrtest02:Test02-First-R1.Motors[7].Contactor1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[7].Contactor1.Super', 'VolPwrtest02:Test02-First-R1.Motors[8].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[8].Super', 'VolPwrtest02:Test02-First-R1.Motors[8].Contactor1.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[8].Contactor1.Super', 'VolPwrtest02:Test02-First-R1.Motors[9].Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[9].Super', 'VolPwrtest02:Test02-First-R1.Motors[9].Contactor2.Super'),
            ('Pwrt02_Component', 'VolPwrtest02:Test02-First-R1.Motors[9].Contactor2.Super', 'None')
            )
        i = 0
        for i in range(len(d)):
            try:
                a = pwrwb.Aref(d[i][1])
                c = pwrwb.Cid(d[i][0])
                n = c.nextAttrObject(a)
                if n is None:
                    if d[i][2] != 'None':
                        self.logger.vlog('E', "Cid.nextAttrObject, No instance should be found, %s",
                                         d[i][1])
                        return
                else:
                    name = n.fullName()
                    if name != d[i][2]:
                        self.logger.vlog('E', "Cid.nextAttrObject, Name doesn't match, %s != %s, idx %s",
                                         name, d[i][2], str(i))
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
            ('Pwrt02_Component',(
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
                c = pwrwb.Cid(d[i][0])
                objectlist = c.attrObjects()
                for a in objectlist:
                    if j >= len(d[i][1]):
                        break
                    if a.name() != d[i][1][j]:
                        self.logger.vlog('E', "Cid.attrObjects, Name doesn't match, %s != %s, idx %s",
                                         a.fullName(), d[i][1][j], str(i))
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
                c = pwrwb.Cid(d[i][0])
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
            ('Test02-Pwrwb-Dv1.InitialValue', '$Boolean', ''),
            ('Test02-Pwrwb-L1.Value', 'Ai', ''),
            ('Test02-Pwrwb-L1.Value.ActualValue', '$Float32', ''),
            ('Test02-Pwrwb-Values.Boolean', '$Boolean', ''),
            ('Test02-Pwrwb-Values.Int8', '$Int8', ''),
            ('Test02-Pwrwb-Values.Int16', '$Int16', ''),
            ('Test02-Pwrwb-Values.Int32', '$Int32', ''),
            ('Test02-Pwrwb-Values.Int64', '$Int64', ''),
            ('Test02-Pwrwb-Values.UInt8', '$UInt8', ''),
            ('Test02-Pwrwb-Values.UInt16', '$UInt16', ''),
            ('Test02-Pwrwb-Values.UInt32', '$UInt32', ''),
            ('Test02-Pwrwb-Values.UInt64', '$UInt64', ''),
            ('Test02-Pwrwb-Values.Float32', '$Float32', ''),
            ('Test02-Pwrwb-Values.Float64', '$Float64', ''),
            ('Test02-Pwrwb-Values.String80', '$String80', ''),
            ('Test02-Pwrwb-Values.Time', '$Time', ''),
            ('Test02-Pwrwb-Values.DeltaTime', '$DeltaTime', ''),
            ('Test02-Pwrwb-Values.Objid', '$Objid', ''),
            ('Test02-Pwrwb-Values.AttrRef', '$AttrRef', '')
            )
        i = 0
        for i in range(len(d)):
            try:                
                a = pwrwb.attribute(d[i][0])
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
             (('In1', 8, 4, 1, 1104), # name, offset, size, elements, flags
              ('In2', 24, 4, 1, 1104),
              ('In3', 40, 4, 1, 1104),
              ('In4', 56, 4, 1, 1104),
              ('In5', 72, 4, 1, 1104),
              ('In6', 88, 4, 1, 1104),
              ('In7', 104, 4, 1, 1104),
              ('In8', 120, 4, 1, 1104),
              ('Status', 124, 4, 1, 1044))),
            ('Pwrt02_Values',
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
                cid = pwrwb.Cid(d[i][0])
                adeflist = cid.attributes()
                j = 0
                for adef in adeflist:
                    if adef.name() == 'PlcNode':
                        continue
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
                    if (adef.flags() & ~(1<<24)) != d[i][1][j][4]:
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
    


ctx = Ctx()
ctx.logger = pwrtest.logger('wb_test-pwrwb', '$pwrp_log/pwrwb.tlog')
try:
    ctx.init()
except:
    quit()
ctx.login()
pwrwb.login('pwrp', 'pwrp')
ctx.volume()
ctx.volumes()
ctx.object()
ctx.attribute()
ctx.getSessionVolume()
ctx.getUser()
ctx.sessionIsEmpty()
ctx.revertSession()
ctx.Oid()
ctx.Oid_child()
ctx.Oid_next()
ctx.Oid_parent()
ctx.Oid_children()
ctx.Oid_name()
ctx.Oid_fullName()
ctx.Oid_oidStr()
ctx.Oid_attribute()
ctx.Aref()
ctx.Aref_name()
ctx.Aref_fullName()
ctx.Aref_arefStr()
ctx.Aref_tid()
ctx.Aref_value()
ctx.Aref_setValue()
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
