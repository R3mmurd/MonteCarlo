CXX = clang++ -std=c++14

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses -Wno-invalid-source-encoding

FLAGS = -g -O0 $(WARN)

OPT= -Ofast $(WARN) -Ofast -Os -static -fvisibility=hidden -fvisibility-inlines-hidden -DNDEBUG -DWITHOUT_NANA -D__extern_always_inline="extern __always_inline"

INCLUDE = -I.

all: MC_pi MC_integration MC_spheres MC_pi-op MC_integration-op MC_spheres-op

MC_pi: MC_pi.C
	$(CXX) $(INCLUDE) $(FLAGS) $@.C -o $@

MC_pi-op: MC_pi.C
	$(CXX) $(INCLUDE) $(OPT) MC_pi.C -o $@

MC_integration: MC_integration.H MC_integration.C
	$(CXX) $(INCLUDE) $(FLAGS) $@.C -o $@

MC_integration-op: MC_integration.H MC_integration.C
	$(CXX) $(INCLUDE) $(OPT) MC_integration.C -o $@

MC_spheres: MC_spheres.C
	$(CXX) $(INCLUDE) $(FLAGS) $@.C -o $@

MC_spheres-op: MC_spheres.C
	$(CXX) $(INCLUDE) $(OPT) MC_spheres.C -o $@

clean:
	$(RM) *~ MC_pi MC_integration MC_spheres MC_pi-op MC_integration-op MC_spheres-op
