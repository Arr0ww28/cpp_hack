# ==============================================================================
# Smart Cabin & Vehicle Health Monitor — Makefile
# ==============================================================================
# Build: make
# Clean: make clean
# Run:   ./vehicle_monitor
# ==============================================================================

CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -Wextra -Wpedantic -O2 -pthread
INCLUDES  = -I./include
SRCDIR    = src
BUILDDIR  = build
TARGET    = vehicle_monitor

# All source modules
SOURCES   = $(SRCDIR)/sensor.cpp \
            $(SRCDIR)/logger.cpp \
            $(SRCDIR)/alert.cpp \
            $(SRCDIR)/dashboard.cpp \
            $(SRCDIR)/main.cpp

OBJECTS   = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# ==============================================================================
# Targets
# ==============================================================================

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean
