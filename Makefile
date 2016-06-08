
ifeq ($(shell uname -m), IP35)
	BUILD = build/$(shell uname -m)
	OBJS_DIR = build/$(shell uname -m)
	LIBDIR = -L/usr/lib64 -L/usr/freeware/lib64
	#LIBDIR = -L/usr/lib64 -L/usr/freeware/lib64 -L/usr/freeware/lib32 -lwwwxml -lxmltok -lxmlparse -lwwwzip -lwwwinit -lwwwapp -lwwwhtml -lwwwtelnet -lwwwnews -lwwwhttp -lwwwmime -lwwwgopher -lwwwftp -lwwwfile -lwwwdir -lwwwcache -lwwwstream -lwwwmux -lwwwtrans -lwwwcore -lwwwutils
	FLAGS = -LANG:std -DIP35 -lxerces-c -DPERFORMANCE -openmp -I/usr/freeware/include/w3c-libwww/
endif
ifeq ($(shell uname -m), IP32)
	BUILD = build/$(shell uname -m)
	OBJS_DIR = build/$(shell uname -m)
	LIBDIR = -L/usr/lib32 -L/usr/freeware/lib32
	FLAGS = -DIP32
endif
ifeq ($(shell uname -m), i686)
	ifeq ($(shell printenv CXX), icc)
		BUILD = build/icc686
		OBJS_DIR = build/icc686
		LIBDIR = -L/usr/local/lib -L/usr/X11R6/lib -lfmod-3.72 -lxerces-c -ldl -lrt -ljpeg -L/usr/lib -lwwwxml -lxmltok -lxmlparse -lwwwzip -lwwwinit -lwwwapp -lwwwhtml -lwwwtelnet -lwwwnews -lwwwhttp -lwwwmime -lwwwgopher -lwwwftp -lwwwfile -lwwwdir -lwwwcache -lwwwstream -lwwwmux -lwwwtrans -lwwwcore -lwwwutils -lmd5 -ldl -lz -lmpeg2 -lmpeg2convert -lezj
		FLAGS = -w1 -DLINUX -fpic -DPERFORMANCE -I/usr/include/ -I/usr/include/w3c-libwww -DHAVE_CONFIG_H 
	else
		BUILD = build/$(shell uname -m)
		OBJS_DIR = build/$(shell uname -m)
		LIBDIR = -L/usr/local/lib -L/usr/X11R6/lib -L/usr/X11R6/lib/modules/extensions -lfmod-3.74 -lxerces-c -ldl -lrt -ljpeg -L/usr/lib -lwwwxml -lxmltok -lxmlparse -lwwwzip -lwwwinit -lwwwapp -lwwwhtml -lwwwtelnet -lwwwnews -lwwwhttp -lwwwmime -lwwwgopher -lwwwftp -lwwwfile -lwwwdir -lwwwcache -lwwwstream -lwwwmux -lwwwtrans -lwwwcore -lwwwutils -lmd5 -ldl -lz 
	 	FLAGS = -Wno-deprecated -Wall -DLINUX -fpic -DPERFORMANCE -I/usr/include/ -I/usr/include/w3c-libwww -DHAVE_CONFIG_H -DGLEW 
	 	#FLAGS = -DDEBUG1 -DDEBUG2 -DDEBUG3 -Wno-deprecated -Wall -DLINUX -fpic -DPERFORMANCE -I/usr/include/ -I/usr/include/w3c-libwww -DHAVE_CONFIG_H -DGLEW
	endif
endif

SOURCE = src

CC = mpiCC
LIBRARIES = -lGLEW -lglut -lGL -lGLU -lm -lX11 -lXmu -lXi -lc 

PROGNAME = main

OBJS = $(OBJS_DIR)/main.o $(OBJS_DIR)/sound/sound.o $(OBJS_DIR)/render/render.o $(OBJS_DIR)/render/posrender.o $(OBJS_DIR)/xmlBase.o $(OBJS_DIR)/config/xmlsax.o $(OBJS_DIR)/config/sax.o $(OBJS_DIR)/config/saxErrosNU.o $(OBJS_DIR)/x3d/x3d.o $(OBJS_DIR)/math.o $(OBJS_DIR)/input.o $(OBJS_DIR)/viewer.o $(OBJS_DIR)/domerror.o $(OBJS_DIR)/texture/texture.o $(OBJS_DIR)/abc.o $(OBJS_DIR)/x3d/x3dsound.o $(OBJS_DIR)/x3d/x3dgprimitives.o $(OBJS_DIR)/joystick.o $(OBJS_DIR)/wand/wand.o $(OBJS_DIR)/wand/wandHeadTracking.o $(OBJS_DIR)/wand/wandSpaceBall.o $(OBJS_DIR)/wand/wandFOBTracker.o $(OBJS_DIR)/wand/posWand.o $(OBJS_DIR)/wand/wandGlove.o $(OBJS_DIR)/sball.o $(OBJS_DIR)/sballserial.o $(OBJS_DIR)/x3d/x3dlights.o $(OBJS_DIR)/engine.o $(OBJS_DIR)/x3d/x3dGeometry3D.o $(OBJS_DIR)/parsehandlers.o $(OBJS_DIR)/ClientSocket.o $(OBJS_DIR)/Socket.o $(OBJS_DIR)/x3d/x3dInterpolation.o $(OBJS_DIR)/camera.o $(OBJS_DIR)/x3d/x3dGrouping.o $(OBJS_DIR)/x3d/x3dShape.o $(OBJS_DIR)/x3d/x3dTime.o $(OBJS_DIR)/x3d/x3dRendering.o $(OBJS_DIR)/render/menu.o $(OBJS_DIR)/x3d/x3dHumanoid.o $(OBJS_DIR)/x3d/x3dTexturing.o $(OBJS_DIR)/x3d/x3dNurbs.o $(OBJS_DIR)/x3d/x3dEnvironmental.o $(OBJS_DIR)/x3d/x3dEnvironmentalSensor.o $(OBJS_DIR)/x3d/x3dCore.o $(OBJS_DIR)/Vector.o $(OBJS_DIR)/x3d/x3dNavigation.o $(OBJS_DIR)/x3d/x3dProto.o $(OBJS_DIR)/render/head.o $(OBJS_DIR)/texture/readrgb.o $(OBJS_DIR)/isense.o $(OBJS_DIR)/www.o $(OBJS_DIR)/texture/movie.o $(OBJS_DIR)/avatars.o $(OBJS_DIR)/render/graphicalInput.o $(OBJS_DIR)/render/graphicalOutput.o $(OBJS_DIR)/render/locks.o $(OBJS_DIR)/render/drawScene.o $(OBJS_DIR)/texture/textureBase.o $(OBJS_DIR)/sphelib.o $(OBJS_DIR)/EngineThreads.o $(OBJS_DIR)/communication.o $(OBJS_DIR)/general.o $(OBJS_DIR)/texture/textureGridPlane.o $(OBJS_DIR)/texture/textureBmp.o $(OBJS_DIR)/texture/texturePpm.o $(OBJS_DIR)/texture/textureRgb.o $(OBJS_DIR)/texture/textureJpeg.o $(OBJS_DIR)/texture/image.o $(OBJS_DIR)/texture/scale.o $(OBJS_DIR)/render/ui.o $(OBJS_DIR)/x3d/x3dNetworking.o $(OBJS_DIR)/commands.o $(OBJS_DIR)/render/calibration.o $(OBJS_DIR)/tesselator.o $(OBJS_DIR)/x3d/x3dScripting.o $(OBJS_DIR)/x3d/x3dFields.o $(OBJS_DIR)/Serial.o $(OBJS_DIR)/x3d/x3dPointingDeviceSensor.o $(OBJS_DIR)/render/ezGLSL.o $(OBJS_DIR)/x3d/x3dShading.o $(OBJS_DIR)/render/Maths/Quaternion.o $(OBJS_DIR)/render/Maths/Vector3D.o $(OBJS_DIR)/render/Maths/Matrix.o  $(OBJS_DIR)/render/Maths/Point3D.o


all : criadir $(BUILD)/$(PROGNAME)
#	cp -f $(BUILD)/$(PROGNAME) /mpi

criadir :
	mkdir -p $(OBJS_DIR)/config
	mkdir -p $(OBJS_DIR)/wand
	mkdir -p $(OBJS_DIR)/render
	mkdir -p $(OBJS_DIR)/render/Maths
	mkdir -p $(OBJS_DIR)/x3d
	mkdir -p $(OBJS_DIR)/texture
	mkdir -p $(OBJS_DIR)/sound

$(BUILD)/$(PROGNAME): $(OBJS)
	$(CC) -g $(FLAGS) -o $@ $(OBJS) $(LIBDIR) $(LIBRARIES)  

$(OBJS_DIR)/%.o:$(SOURCE)/%.cpp
	$(CC) -g -c $(FLAGS) -o $@ $?

clean:
	rm -f $(OBJS) $(BUILD)/main
