CXXFLAGS = -g -fpic

expression: expression.o parser.o tokenizer.o utils.o
	$(CXX) expression.o parser.o tokenizer.o utils.o -o expression

clean:
	rm -rf *.o expression

expression.o: parser.h tokenizer.h utils.h
parser.o: parser.h tokenizer.h utils.h
tokenizer.o: tokenizer.h
utils.o: utils.h
