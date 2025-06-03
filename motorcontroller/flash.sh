set -e

ELF_PATH="build/Debug/motorcontroller.elf"

INTERFACE_CFG="interface/stlink.cfg"

TARGET_CFG="target/stm32f4x.cfg"

openocd \
	-f $INTERFACE_CFG \
	-f $TARGET_CFG \
        -c "program $ELF_PATH verify reset exit"
