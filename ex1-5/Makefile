EX=ex1 ex2 ex3 ex4 ex5
all: $(EX)

#  Unix/Linux
%: %.c;gcc -Wall -O3 -o $@ $< -lglut -lGLU

#  OSX
#%: %.c;gcc -Wall -O3 -o $@ $< -framework GLUT -framework OpenGL

#  MinGW
#%: %.c;gcc -Wall -O3 -o $@ $< -lglut32cu -lglu32 -lopengl32

clean:
	rm -f $(EX)
