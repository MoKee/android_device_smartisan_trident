cmd_usr/include/uapi/.install := /bin/sh /Volumes/android/mokee/kernel/smartisan/sdm845/scripts/headers_install.sh ./usr/include/uapi /Volumes/android/mokee/kernel/smartisan/sdm845/include/uapi ; /bin/sh /Volumes/android/mokee/kernel/smartisan/sdm845/scripts/headers_install.sh ./usr/include/uapi /Volumes/android/mokee/kernel/smartisan/sdm845/include ; /bin/sh /Volumes/android/mokee/kernel/smartisan/sdm845/scripts/headers_install.sh ./usr/include/uapi ./include/generated/uapi ; for F in ; do echo "\#include <asm-generic/$$F>" > ./usr/include/uapi/$$F; done; touch usr/include/uapi/.install