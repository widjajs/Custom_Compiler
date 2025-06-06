CXX = g++
OBJS = main.o tokenizer.o error.o parser.o
TARGET = main
DEPS = tokenizer.hpp error.hpp parser.hpp

%.o: src/%.cpp $(addprefix includes/,$(DEPS))
	$(CXX) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
