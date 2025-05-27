CXX = g++
BJS = main.o tokenizer.o error.o
TARGET = main
DEPS = tokenizer.hpp error.hpp

%.o: src/%.cpp $(addprefix includes/,$(DEPS))
	$(CXX) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
