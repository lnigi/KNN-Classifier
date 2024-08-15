#program compilation and cleaning of object files
KNNClassifier: KNN_main.cc lib/Document.cc lib/Document.h lib/KNNClassifier.cc lib/KNNClassifier.h lib/TrainingSet.cc lib/TrainingSet.h
	g++ -c KNN_main.cc lib/Document.cc lib/KNNClassifier.cc lib/TrainingSet.cc
	g++ KNN_main.o Document.o KNNClassifier.o TrainingSet.o -o KNNClassifier
	rm -f *.o

#program compilation only, no cleaning of object files
KNNClassifier-no-delete: KNN_main.cc lib/Document.cc lib/Document.h lib/KNNClassifier.cc lib/KNNClassifier.h lib/TrainingSet.cc lib/TrainingSet.h
	g++ -c KNN_main.cc lib/Document.cc lib/KNNClassifier.cc lib/TrainingSet.cc
	g++ KNN_main.o Document.o KNNClassifier.o TrainingSet.o -o KNNClassifier

#cleaning of object files
clean-obj:
	rm -f *.o

#clean all
clean-all:
	rm -f KNNClassifier *.o