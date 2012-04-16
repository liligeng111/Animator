# Makefile
# Generated by Makefile Generater at Mon Apr 16 15:36:38 2012

TARGET = main
CC = g++
OBJS = particlesystem.o pointobj.o rulerwindow.o robotarm.o rect.o point.o particle.o modelerview.o modeleruiwindows.o modelerui.o modelerdraw.o modelerapp.o linearcurveevaluator.o indicatorwindow.o graphwidget.o curveevaluator.o curve.o color.o camera.o bitmap.o animatoruiwindows.o
SRCS = particlesystem.cpp pointobj.cpp rulerwindow.cpp robotarm.cpp rect.cpp point.cpp particle.cpp modelerview.cpp modeleruiwindows.cxx modelerui.cpp modelerdraw.cpp modelerapp.cpp linearcurveevaluator.cpp indicatorwindow.cpp graphwidget.cpp curveevaluator.cpp curve.cpp color.cpp camera.cpp bitmap.cpp animatoruiwindows.cxx
CXXFLAGS = $(shell fltk-config --use-gl --use-images --cxxflags ) -I. 
LDFLAGS = $(shell fltk-config --use-gl --use-images --ldflags ) 
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags ) 
DEBUG = -g
LINK = $(CC)

# LINK = $(shell fltk-config --use-gl --compile)

.SUFFIXES: .o .cpp .cxx
%.o: %.cpp
	$(CC) $(DEBUG) $(CXXFLAGS) -c $<

%.o: %.cxx
	$(CC) $(DEBUG) $(CXXFLAGS) -c $<

all: $(TARGET)
	$(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)

$(TARGET): $(OBJS)
modeleruiwindows.cxx : modeleruiwindows.h modelerview.h rulerwindow.h graphwidget.h indicatorwindow.h rect.h point.h curve.h curveevaluator.h
robotarm.cpp : modelerview.h modelerapp.h modelerdraw.h particlesystem.h vec.h
animatoruiwindows.cxx : animatoruiwindows.h indicatorwindow.h rulerwindow.h graphwidget.h modelerview.h rect.h point.h curve.h curveevaluator.h
particlesystem.cpp : particlesystem.h vec.h
rulerwindow.cpp : rulerwindow.h
linearcurveevaluator.cpp : linearcurveevaluator.h curveevaluator.h curve.h point.h
pointobj.cpp : 
curveevaluator.cpp : curveevaluator.h curve.h point.h
particle.cpp : 
modelerapp.cpp : particlesystem.h modelerapp.h modelerview.h modelerui.h camera.h vec.h modelerdraw.h modeleruiwindows.h mat.h rect.h point.h curve.h curveevaluator.h rulerwindow.h graphwidget.h indicatorwindow.h
indicatorwindow.cpp : indicatorwindow.h
modelerview.cpp : modelerview.h camera.h bitmap.h modelerapp.h particlesystem.h vec.h mat.h rect.h point.h curve.h curveevaluator.h
color.cpp : color.h
rect.cpp : rect.h
modelerui.cpp : modelerui.h camera.h modelerview.h modelerdraw.h modelerapp.h particlesystem.h modeleruiwindows.h vec.h mat.h rect.h point.h curve.h curveevaluator.h rulerwindow.h graphwidget.h indicatorwindow.h
curve.cpp : curve.h curve.h curveevaluator.h point.h
modelerdraw.cpp : modelerdraw.h
graphwidget.cpp : graphwidget.h linearcurveevaluator.h rect.h point.h curve.h curveevaluator.h
point.cpp : point.h
camera.cpp : camera.h curve.h curveevaluator.h linearcurveevaluator.h vec.h mat.h rect.h point.h
bitmap.cpp : bitmap.h

clean:
	@rm $(OBJS) -vf
	@rm $(TARGET) -vf
	@echo "Clean successfully"

# end of Makefile
