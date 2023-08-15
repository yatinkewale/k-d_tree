# --------------------------------------------
# Compiler
# --------------------------------------------
#
CC = gcc

# --------------------------------------------
# Flags
# --------------------------------------------
#
# Compiler Flags
CFLAGS = -c -Wall -Werror	# Flags to stop compiling at assembly, enable\
				# all warnings into errors
FPIC   = -fpic			# Position independent code flag

# Linker Flags
MATH   = -lm			# Math.h library flag
O      = -o			# Output flag
STATIC = -static		# For linking static libraries only
SHARED = -shared		# For linking dynamic libraries only
AR     =  ar			# Archive Flag
RC     = -rc			# Create and replace archive without warnings
L      = -L			# Current Directory path flag
directory = .			# Current path where all files are located
library = -lkdtree		# library target created

# --------------------------------------------
# Targets
# --------------------------------------------
#
# Object file targets
kdtree = kdtree.o		# Object file for  kdtree.c
main   = main.o			# Object file for  main.c

# Headers files
headers = kdtree.h kd_internals.h

# libraries for targets
shared_target = libkdtree.so
static_target = libkdtree.a


# --------------------------------------------
# Basic 'make' target
# --------------------------------------------
#
basic: $(kdtree) $(main)
	$(CC) $^ $(O) basic $(MATH)
	@echo "\nCreating ./basic executable from obj files. Run -> ./basic your_file.csv x y\
		\n\t Type following commands for more builds\
		\n\t 1. For creating shared library. Run -> libshared\
		\n\t 2. For creating static library. Run -> libstatic\
		\n\t 3. For using shared library.    Run -> shared\
		\n\t 4. For using static library.    Run -> static"
	
# --------------------------------------------
# Custom 'make' targets
# --------------------------------------------
# Build libkdtree as a static library
.PHONY: libstatic
libstatic: $(kdtree) $(headers)
	$(AR) $(RC) $(static_target) $<
	@echo "- - - - - Built libkdtree as a static library - - - - -\n"

# Build libkdtree as a shared library
.PHONY: libshared
libshared: $(kdtree) $(headers)
	$(CC) $(SHARED) $< $(O) $(shared_target)
	@echo "- - - - - Built libkdtree as a shared library - - - - -\n"

# Build 'main' with static linking to libkdtree
.PHONY: static
static: libstatic
static: $(main)
	$(CC) $< $(L)$(directory) $(library) $(O) $@ $(MATH)
	@echo "\nCreating executable from static library. Run -> ./static your_file.csv x y"

# Build 'main' with dynamic linking to libkdtree
.PHONY: shared
shared: libshared
shared: $(main)
	$(CC) $< $(L)$(directory) $(library) $(O) $@ $(MATH)
	@echo "\nCreating executable from shared library. Run -> ./shared your_file.csv x y"
	
# Bonus: Create a 'make clean' target to remove all build-generated files
.PHONY: clean
clean: 
	rm -f *.o *~ *.so *. *.a *.gch shared static basic
	@echo "- - - - - Destroyed all build generated files - - - - -"

# --------------------------------------------
# Implicit build rules
# --------------------------------------------
# 
# Build object files
$(kdtree): kdtree.c $(headers)
	$(CC) $(CFLAGS) $(FPIC) $<

$(main): main.c $(headers)
	$(CC) $(CFLAGS) $<
