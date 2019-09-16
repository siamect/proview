import os

class Error(Exception):
    def __init__(self, status, str):
        self.status = status
        self.str = str

    def __str__(self):
        return self.str
    
def translateFilename(name):
    if name[:1] == '$':
        idx = name.find('/')
        env = os.environ.get(name[1:idx])
        if env == None:
            return name
        nname = env + name[idx:]                     
        return nname
    else:
        return name
    
def reTranslateFilename(name):
    idx = name.rfind('/')
    if idx == -1:
        return name
    idx2 = name[:idx].rfind('/')
    if idx2 == -1:
        return name
    
    dir = name[idx2+1:idx]
    if name[:idx] == os.environ.get('pwr_' + dir):
        return '$pwr_' + dir + name[idx:]
    elif name[:idx] == os.environ.get('pwrp_' + dir):
        return '$pwrp_' + dir + name[idx:]
    else:
        return name
