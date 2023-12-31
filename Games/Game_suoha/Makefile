#
#Makefile c/c++编译脚本
#
#https://blog.csdn.net/hust_keven/article/details/8943909

#编译选项
CROSS_COMPILE =
#编译优化 -O2 debug -g
OPTIMIZE := -O2 -g
#禁用警告
WARNINGS := -w #-Wall -Wno-unused -Wno-format -Wno-switch -Wno-return-type -Wno-unused-variable \
#-Wno-sizeof-pointer-memaccess -Wno-narrowing -Wno-sign-compare -Wno-maybe-uninitialized -Wno-pointer-to-int-cast
DEFS     := -DMYDEF=1 -UMYDEF2
EXTRA_CFLAGS := -Wfatal-errors #-Wno-overflow -Wno-sign-conversion -Wformat-security

#编译.o文件中间目录，必须bin
OBJ_DIR = bin

#编译目标文件路径/文件名
TARGET := ../../bin/libGame_Suoha.so

#编译.a静态库
#TARGET_TYPE  := ar
#编译bin可执行文件
#TARGET_TYPE  := app
#编译.so动态库
TARGET_TYPE := so

#编译文件类型
SUFFIX = c cpp cc cxx

#包含头文件目录
INC_DIR =. \
/usr/include \
/usr/local/include \
/usr/local/include/google/protobuf \
/usr/local/include/bsoncxx/v_noabi \
/usr/local/include/mongocxx/v_noabi \
../.. \
../../proto \
../../Framework \
../../Framework/public \
../../Framework/GameServer \
../../Framework/GameServer/public \
../../Framework/thirdpart \
../../jsoncpp/include \

#包含源文件目录
SRC_DIR =. \
../.. \

#附加编译依赖文件
EXTRA_SRC =. \
../../Framework/public/GlobalFunc.h \
../../Framework/public/GlobalFunc.cpp \
../../Framework/public/ToolTime.h \
../../Framework/GameServer/public/ITableFrame.h \
../../Framework/GameServer/public/ITableFrameSink.h \
../../Framework/GameServer/public/IAndroidUserItemSink.h \
../../Framework/GameServer/GameServerDefine.h \
../../Framework/GameServer/ServerUserItem.h \
../../proto/suoha.Message.pb.h \
../../proto/suoha.Message.pb.cc \


#排除掉不编译的文件集合
EXCLUDE_FILES = \
pb2Json.h pb2Json.cpp ./main.cpp \

#链接器附加库目录/输入库依赖项
LDFLAGS += -pthread -lrt -lm -lz -ldl \
-lfreetype -ljpeg -lpng -lgd -lssl -lcurl \
-lglog \
-lprotobuf \
-lboost_date_time -lboost_filesystem -lboost_system -lboost_thread \
-lbsoncxx -lmongocxx \
-L../../Framework/libs -lmuduo \
-ljson_linux-gcc-4.8.5_libmt \

define add_newline
$1

endef

define set_src_x
src-$1 = $(filter-out $4,$(foreach d,$2,$(wildcard $d/*.$1)) $(filter %.$1,$3))

endef

define set_obj_x
obj-$1 = $(patsubst %.$1,$3%.o,$(notdir $2))

endef

ifneq ($(OBJ_DIR),)
prefix_objdir  = $(shell echo $(OBJ_DIR)|sed 's:\(\./*\)*::')
prefix_objdir := $(filter-out /,$(prefix_objdir)/)
endif

GCC      := $(CROSS_COMPILE)gcc
G++      := $(CROSS_COMPILE)g++
src_dir  := $(sort . $(SRC_DIR))
inc_dir  := $(foreach d,$(sort $(INC_DIR) $(SRC_DIR)),-I$d)

$(eval $(foreach i,$(SUFFIX),$(call set_src_x,$i,$(src_dir),$(EXTRA_SRC),$(EXCLUDE_FILES))))
$(eval $(foreach i,$(SUFFIX),$(call set_obj_x,$i,$(src-$i),$(prefix_objdir))))
$(eval $(foreach f,$(EXTRA_SRC),$(call add_newline,vpath $(notdir $f) $(dir $f))))
$(eval $(foreach d,$(src_dir),$(foreach i,$(SUFFIX),$(call add_newline,vpath %.$i $d))))

all_objs = $(foreach i,$(SUFFIX),$(obj-$i))
all_srcs = $(foreach i,$(SUFFIX),$(src-$i))

CFLAGS       = $(EXTRA_CFLAGS) $(WARNINGS) $(OPTIMIZE) $(DEFS) -std=c++11
TARGET_TYPE := $(strip $(TARGET_TYPE))

#检查编译归档类型有效性(*.so *.a bin)
ifeq ($(filter $(TARGET_TYPE),so ar app),)
$(error Unexpected TARGET_TYPE `$(TARGET_TYPE)')
endif

ifeq ($(TARGET_TYPE),so)
 CFLAGS  += -fpic -shared
 LDFLAGS += -shared
endif

PHONY = all .mkdir clean

all: .mkdir $(TARGET)
	@echo ""
ifeq ($(TARGET_TYPE),so)
	ldd -r $(TARGET)
	@echo "输出动态库文件 $(TARGET) [OK]"
else ifeq ($(TARGET_TYPE),app)
	ldd -r $(TARGET)
	@echo "输出可执行文件 $(TARGET) [OK]"
else ifeq ($(TARGET_TYPE),ar)
	nm -g --defined-only $(TARGET)
	@echo "输出静态库文件 $(TARGET) [OK]"
endif
	@echo ""
	
define cmd_o
$$(obj-$1): $2%.o: %.$1  $(MAKEFILE_LIST)
	$(GCC) $(inc_dir) -Wp,-MT,$$@ -Wp,-MMD,$$@.d $(CFLAGS) -c -o $$@ $$<

endef
$(eval $(foreach i,$(SUFFIX),$(call cmd_o,$i,$(prefix_objdir))))

ifeq ($(TARGET_TYPE),ar)
$(TARGET): AR := $(CROSS_COMPILE)ar
$(TARGET): $(all_objs)
	rm -f $@
	$(AR) rcvs $@ $(all_objs)
else
$(TARGET): LD = $(if $(strip $(src-cpp) $(src-cc) $(src-cxx)),$(G++),$(GCC))
$(TARGET): $(all_objs)
	$(LD) $(all_objs) $(LDFLAGS) -o $@
endif

.mkdir:
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi

clean:
	rm -f $(prefix_objdir)*.o $(prefix_objdir)*.o.d $(TARGET)

-include $(patsubst %.o,%.o.d,$(all_objs))

.PHONY: $(PHONY)

