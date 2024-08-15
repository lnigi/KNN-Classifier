<h1>KNN Classifier</h1>
This is the code of my project for the course of Programming and Software Design.

<h2>Files description</h2>

<ul>
  <li>"to_analyze1.txt" and "to_analyze2.txt" are example texts to test if the program is working correctly</li>
  <li>"KNN_full.cc" is an aggregation of all the code into a single file.</li>
  <li>"makefile" can be used to generate an executable, compiling every separate file.</li>
  <li>"TrainingSet" contains files used to train the program. Files can be added to be used for training. WARNING: Make sure that the first line includes the target classification for the following text.</li>
</ul>

<h2>Program Usage</h2>

<h3>Creating the executable</h3>

Before running the downloaded program, make sure to make "makefile" executable:

```
chmod +x makefile
```
To create the executable, use one of the following commands:
```
make KNNClassifier   ##create executable REMOVING .o files 
```
```
make KNNClassifier-no-delete   ##create executable WITHOUT removing .o files
```
To delete ".o" files:
```
make clean-obj
```
To delete the executable as well:
```
make clean-all
```

<h3>Running the program</h3>
Once the executable has been created, the following syntax can be used to run the program:

```
./KNNClassifier to_analyze1.txt to_analyze2.txt ... to_analyzeN.txt
```
NOTE: One or more files can be classified with a single execution of the program, as shown in the code above
