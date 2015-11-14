#FORWINDOW              = TRUE

Packages 		       = \
	                      Calc \
	                      Action \
	                      Gui \

Empty                  =
Space                  = $(Empty) $(Empty)
temproot               = $(subst /Source,$(Space),$(CURDIR))
DEV_ROOT               = $(word 1,$(temproot))

ObjectFiles            = $(wildcard Object/*/*.o) 
MainProg               = main.c
Main                   = imagerecog

makelinux:
	@echo "Start Make Linux"

PackageListLoop        = $(patsubst %,Source/%/.loop,$(Packages))

ifdef FORWINDOW
LinkerOption           = $(CLIBS) -lgsl -lgslcblas
else
LinkerOption           = `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0` -lm -lgsl -lgslcblas
COPTION                = `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0`
endif


%.loop:
	@$(MAKE) $(MakeOptions) -C $(subst .loop,,$@) -f Make package_$(MAKECMDGOALS)

build: makelinux $(PackageListLoop)
	@echo "End Make Linux"
	@gcc -c Source/main.c -o Object/main.o $(COPTION)
	@gcc -g -o $(DEV_ROOT)/Binary/$(Main).exe $(ObjectFiles) Object/main.o $(LinkerOption)
	$(DEV_ROOT)/Binary/$(Main).exe

clean: $(PackageListLoop)
