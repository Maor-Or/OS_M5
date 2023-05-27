CC = clang++-14
FLAGS = -Wall

all:  st_queue.so st_activeObject.so st_pipeline

st_queue.so: st_queue.cpp st_queue.hpp
	$(CC) -shared -fPIC st_queue.cpp -o st_queue.so -pthread

st_activeObject.so: st_activeObject.cpp st_activeObject.hpp st_queue.hpp st_queue.so
	$(CC) -shared -fPIC st_activeObject.cpp -o st_activeObject.so ./st_queue.so -pthread

st_pipeline: st_pipeline.cpp st_activeObject.so
	$(CC) $(FLAGS) st_pipeline.cpp -o st_pipeline ./st_activeObject.so ./st_queue.so -pthread

.PHONY: clean all

clean:
	rm -f *.o *.so st_pipeline