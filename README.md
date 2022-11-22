# Rebeca-Timed-Actors
The implementations of my Bachelor's thesis project, named `Generation of efficient state space for Rebeca models extended with timer variables`.


## Abstract
In order to model various communication systems such as programs and tools used in medical industry, a handful of communication patterns have
already been developed. These patters aim to model the communication-based requirements of said systems. One of the most crucial requirements is
time limit of the system and its components. In most previously carried out studies, including the main reference of this study, the model is first converted into a transition system and then needed properties are checked using the generated state space. In this study, however, we aim to extend current tool with timer variables and perform the aforementioned analysis as the state space is being generated. This way, an on-the-fly model checking is introduced. Our methodology is to add timer variables to Afra, which is the main tool used by previous studies. This novel approach is a more effective model checking method for medical-based systems. Also, performing on-the-fly model checking results in efficient use of resources and thus, enables us to model more complex systems.

## Technologies
First, I had to get a good understanding of Rebeca modeling language, which is very similar to `Java`. Then, I made changes to the compiled `C++` model checking codes to account for timed-difference between states. 


## Best thesis award
Unfortunately, this one is in Persian!


<img src="https://github.com/maancham/Rebeca-Timed-Actors/blob/main/bachelor%20nomination.jpg" width="500" height="600">
