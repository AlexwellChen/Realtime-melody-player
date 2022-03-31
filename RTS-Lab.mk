##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=RTS-Lab
ConfigurationName      :=Debug
WorkspacePath          :=/home/jay/TinyTimber
ProjectPath            :=/home/jay/TinyTimber/RTS-Lab
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jay
Date                   :=23/02/22
CodeLitePath           :=/home/jay/.codelite
LinkerName             :=/home/jay/GCC/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++
SharedObjectLinkerName :=/home/jay/GCC/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).elf
Preprocessors          :=$(PreprocessorSwitch)STM32F40_41xxx 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RTS-Lab.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  --specs=nano.specs -mthumb -o $(IntermediateDirectory)/$(ProjectName).elf  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -nostartfiles -T$(ProjectPath)/md407-ram.x -Wl,-Map=$(IntermediateDirectory)/$(ProjectName).map,--cref
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)$(ProjectPath) $(IncludeSwitch)$(ProjectPath)/device/inc $(IncludeSwitch)$(ProjectPath)/driver/inc 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /home/jay/GCC/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-ar rcu
CXX      := /home/jay/GCC/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++
CC       := /home/jay/GCC/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fverbose-asm $(Preprocessors)
ASFLAGS  := 
AS       := /home/jay/GCC/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix) $(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix) $(IntermediateDirectory)/dispatch.s$(ObjectSuffix) $(IntermediateDirectory)/application.c$(ObjectSuffix) $(IntermediateDirectory)/ToneGenerator.c$(ObjectSuffix) $(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix) \
	$(IntermediateDirectory)/sioTinyTimber.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix) $(IntermediateDirectory)/Note.c$(ObjectSuffix) $(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix) $(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	arm-none-eabi-objcopy -S -O srec  ./Debug/RTS-Lab.elf ./Debug/RTS-Lab.s19
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix): driver/src/stm32f4xx_syscfg.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(DependSuffix) -MM driver/src/stm32f4xx_syscfg.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_syscfg.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(PreprocessSuffix): driver/src/stm32f4xx_syscfg.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_syscfg.c$(PreprocessSuffix) driver/src/stm32f4xx_syscfg.c

$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix): driver/src/stm32f4xx_exti.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(DependSuffix) -MM driver/src/stm32f4xx_exti.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_exti.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(PreprocessSuffix): driver/src/stm32f4xx_exti.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_exti.c$(PreprocessSuffix) driver/src/stm32f4xx_exti.c

$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix): driver/src/stm32f4xx_can.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(DependSuffix) -MM driver/src/stm32f4xx_can.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_can.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(PreprocessSuffix): driver/src/stm32f4xx_can.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_can.c$(PreprocessSuffix) driver/src/stm32f4xx_can.c

$(IntermediateDirectory)/startup.c$(ObjectSuffix): startup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/startup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/startup.c$(DependSuffix) -MM startup.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/startup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup.c$(PreprocessSuffix): startup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup.c$(PreprocessSuffix) startup.c

$(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix): sciTinyTimber.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sciTinyTimber.c$(DependSuffix) -MM sciTinyTimber.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/sciTinyTimber.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sciTinyTimber.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sciTinyTimber.c$(PreprocessSuffix): sciTinyTimber.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sciTinyTimber.c$(PreprocessSuffix) sciTinyTimber.c

$(IntermediateDirectory)/dispatch.s$(ObjectSuffix): dispatch.s
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/dispatch.s$(ObjectSuffix) -MF$(IntermediateDirectory)/dispatch.s$(DependSuffix) -MM dispatch.s
	$(AS) "/home/jay/TinyTimber/RTS-Lab/dispatch.s" $(ASFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/dispatch.s$(ObjectSuffix) -I$(IncludePath)
$(IntermediateDirectory)/dispatch.s$(PreprocessSuffix): dispatch.s
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/dispatch.s$(PreprocessSuffix) dispatch.s

$(IntermediateDirectory)/application.c$(ObjectSuffix): application.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/application.c$(ObjectSuffix) -MF$(IntermediateDirectory)/application.c$(DependSuffix) -MM application.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/application.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/application.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/application.c$(PreprocessSuffix): application.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/application.c$(PreprocessSuffix) application.c

$(IntermediateDirectory)/ToneGenerator.c$(ObjectSuffix): ToneGenerator.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ToneGenerator.c$(ObjectSuffix) -MF$(IntermediateDirectory)/ToneGenerator.c$(DependSuffix) -MM ToneGenerator.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/ToneGenerator.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ToneGenerator.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ToneGenerator.c$(PreprocessSuffix): ToneGenerator.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ToneGenerator.c$(PreprocessSuffix) ToneGenerator.c

$(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix): TinyTimber.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix) -MF$(IntermediateDirectory)/TinyTimber.c$(DependSuffix) -MM TinyTimber.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/TinyTimber.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TinyTimber.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TinyTimber.c$(PreprocessSuffix): TinyTimber.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TinyTimber.c$(PreprocessSuffix) TinyTimber.c

$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix): driver/src/stm32f4xx_usart.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(DependSuffix) -MM driver/src/stm32f4xx_usart.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_usart.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(PreprocessSuffix): driver/src/stm32f4xx_usart.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_usart.c$(PreprocessSuffix) driver/src/stm32f4xx_usart.c

$(IntermediateDirectory)/sioTinyTimber.c$(ObjectSuffix): sioTinyTimber.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sioTinyTimber.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sioTinyTimber.c$(DependSuffix) -MM sioTinyTimber.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/sioTinyTimber.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sioTinyTimber.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sioTinyTimber.c$(PreprocessSuffix): sioTinyTimber.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sioTinyTimber.c$(PreprocessSuffix) sioTinyTimber.c

$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix): driver/src/stm32f4xx_dac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(DependSuffix) -MM driver/src/stm32f4xx_dac.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_dac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(PreprocessSuffix): driver/src/stm32f4xx_dac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_dac.c$(PreprocessSuffix) driver/src/stm32f4xx_dac.c

$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix): driver/src/stm32f4xx_gpio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(DependSuffix) -MM driver/src/stm32f4xx_gpio.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_gpio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(PreprocessSuffix): driver/src/stm32f4xx_gpio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_gpio.c$(PreprocessSuffix) driver/src/stm32f4xx_gpio.c

$(IntermediateDirectory)/Note.c$(ObjectSuffix): Note.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Note.c$(ObjectSuffix) -MF$(IntermediateDirectory)/Note.c$(DependSuffix) -MM Note.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/Note.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Note.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Note.c$(PreprocessSuffix): Note.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Note.c$(PreprocessSuffix) Note.c

$(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix): canTinyTimber.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix) -MF$(IntermediateDirectory)/canTinyTimber.c$(DependSuffix) -MM canTinyTimber.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/canTinyTimber.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/canTinyTimber.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/canTinyTimber.c$(PreprocessSuffix): canTinyTimber.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/canTinyTimber.c$(PreprocessSuffix) canTinyTimber.c

$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix): driver/src/stm32f4xx_rcc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(DependSuffix) -MM driver/src/stm32f4xx_rcc.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_rcc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(PreprocessSuffix): driver/src/stm32f4xx_rcc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_rcc.c$(PreprocessSuffix) driver/src/stm32f4xx_rcc.c

$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix): driver/src/stm32f4xx_tim.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix) -MF$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(DependSuffix) -MM driver/src/stm32f4xx_tim.c
	$(CC) $(SourceSwitch) "/home/jay/TinyTimber/RTS-Lab/driver/src/stm32f4xx_tim.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(PreprocessSuffix): driver/src/stm32f4xx_tim.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/driver_src_stm32f4xx_tim.c$(PreprocessSuffix) driver/src/stm32f4xx_tim.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


