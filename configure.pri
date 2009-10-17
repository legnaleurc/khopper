# paths
unix:ROOT_PATH = $$system(pwd)
win32:ROOT_PATH = $$system(cd)
CORE_PATH = $${ROOT_PATH}/core

# project
VERSION  = 0.2.60
DEFINES += KHOPPER_VERSION=$${VERSION}
