CXXFLAGS = -g

expression: expression.o tokenizer.o
	$(CXX) expression.o tokenizer.o -o expression

expression.o: tokenizer.h
tokenizer.o: tokenizer.h
