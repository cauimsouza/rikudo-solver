# Project settings
BIN_DIR = bin
BUILD_DIR = build
JAVA_SRC_DIR = src/gui
JAVA_EXECUTABLE = gui.jar
CPP_HEADER_DIR = src/rikudo_solver
CPP_SRC_DIR = src/rikudo_solver
CPP_EXECUTABLE = RikudoSolver

# Create build directories
$(shell mkdir -p $(BUILD_DIR)) # create directories for object files
$(shell mkdir -p $(BIN_DIR)) # create directories for binary files

# Gather filenames

CPP_SOURCES = $(shell find $(CPP_SRC_DIR) -name '*.cpp') # cpp files 
CPP_OBJECTS = $(CPP_SOURCES:$(CPP_SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) # replace .cpp with .o
JAVA_SOURCES = $(shell find $(JAVA_SRC_DIR) -name '*.java') # java files 
JAVA_MAIN = Rikudo
# Compiler settings

CXX := g++ 
CXX_FLAGS := -std=c++11 -I $(CPP_HEADER_DIR)
JC = javac


all: $(BIN_DIR)/$(CPP_EXECUTABLE) $(BIN_DIR)/$(JAVA_EXECUTABLE)

# C++ recipes

$(BIN_DIR)/$(CPP_EXECUTABLE): $(CPP_OBJECTS)
	$(CXX) $(CXX_FLAGS) $^ -o $@	

$(BUILD_DIR)/%.o: $(CPP_SRC_DIR)/%.cpp # source files
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

# Java recipes

$(BIN_DIR)/$(JAVA_EXECUTABLE): $(JAVA_SOURCES)
	$(JC) -cp $(BUILD_DIR) -d $(BUILD_DIR) $(JAVA_SRC_DIR)/*.java
	cd $(BUILD_DIR) && jar cfe ../$@ $(JAVA_MAIN) *.class && cd -

.PRECIOUS: $(BUILD_DIR)/%.o $(BUILD_DIR)/%.class  # dont delete intermediary object files

clean:
	@$(RM) -r $(BUILD_DIR)
	@$(RM) $(BIN_NAME)
	@$(RM) -r $(BIN_DIR)