cmd_usr/include/scsi/ufs/.install := /bin/sh /Volumes/android/mokee/kernel/smartisan/sdm845/scripts/headers_install.sh ./usr/include/scsi/ufs /Volumes/android/mokee/kernel/smartisan/sdm845/include/uapi/scsi/ufs ioctl.h ufs.h; /bin/sh /Volumes/android/mokee/kernel/smartisan/sdm845/scripts/headers_install.sh ./usr/include/scsi/ufs /Volumes/android/mokee/kernel/smartisan/sdm845/include/scsi/ufs ; /bin/sh /Volumes/android/mokee/kernel/smartisan/sdm845/scripts/headers_install.sh ./usr/include/scsi/ufs ./include/generated/uapi/scsi/ufs ; for F in ; do echo "\#include <asm-generic/$$F>" > ./usr/include/scsi/ufs/$$F; done; touch usr/include/scsi/ufs/.install