CXXFLAGS = -g -fpic

expression: expression.o main.o parser.o tokenizer.o utils.o
	$(CXX) expression.o main.o parser.o tokenizer.o utils.o -o expression

clean:
	rm -rf *.o expression

expression.o: expression.h parser.h tokenizer.h utils.h
main.o: expression.h parser.h tokenizer.h utils.h
parser.o: expression.h parser.h tokenizer.h utils.h
tokenizer.o: tokenizer.h
utils.o: utils.h
