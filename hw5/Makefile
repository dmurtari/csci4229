hw=hw5
all: $(hw)

#  Unix/Linux
%: %.c;gcc -Wall -O3 -o $@ $< -lglut -lGLU -lGL -lm

#  OSX
#%: %.c;gcc -Wall -O3 -o $@ $< -framework GLUT -framework OpenGL

#  MinGW
#%: %.c;gcc -Wall -O3 -o $@ $< -lglut32cu -lglu32 -lopengl32

clean:
	rm -f $(hw)
