README
Catherine McLean and Charles Zheng
cmclean and charlez

Explanation of Design:

Model represents the actual music sheet. Its implementation,
ModelImpl, has fields that keep track of the current time stamp that
the model is at, how many beats are in a measure, the tempo, and all
of the Notes in the piece. It also keeps track of the highest and
lowest pitches of the piece, and both the final start beat and 
the final end beat that occurs in the piece. Currently, the timestamp
field is not called by the view, but we anticipate that it will be
very useful when we implement a controller that supports play/pause
functionality. 

We chose to represent the Notes as a HashMap that maps the list of
Notes that start at a given time to that time. We chose this because an 
important feature of the music sheet is being able to play all of the notes 
at the next time at once. We can find the notes to be played at any given 
time using the getNotesAtTime() method. We can add a single note with addNote(). 
We can edit or delete notes using the methods deleteNote(),
editNoteStartTime(), and editNoteEndTime(). We can play two pieces of
music simultaneously or consecutively by sending a list of all the
notes in the piece to the method addAllNotes() with a boolean saying
whether they should be added at the end, or simultaneously.

My Note interface is implemented in MusicNote. MusicNote keeps track
of the pitch, which is an interface with an int value that represents
how many steps from C at octave -1 the Note is at and a string with its actual 
letter note and octave number, the start time of the Note, and the end time
of the Note. Notes and pitches are immutable. 

We can change the start and end times of notes in our model, 
however we do not use this yet. We plan to utilize these methods 
once we implement the controller.

We implemented a Console view that prints the music sheet with the pitches 
across the horizontal axis, and the timestamps going down. This makes
it easy to scroll. We used a Model to View adapter for this, which we
describe below.

For our GuiView, we chose to use Swing. Our concrete class is GuiViewFrame, which is
a Frame, and it takes in a ConcreteGuiViewPanel, which is a JPanel, as data. 
We used a JScrollPane to decorate our JPanel and add scrolling functionality. 
Notes are represented with a black square at its start time and green squares for
each beat for which it is sustained. Pitches are spread along the vertical axis, 
and time is along the horizontal axis. 

To create our mock MidiDevice and mock Receiver, we added fields to our MidiViewImpl
concrete class that represented the logged output, and whether or not we were in
testing mode. If we were in testing mode, then we set the PrintStream of 
System.out to be a ByteArrayOutputStream, which we could then keep track of and test
as a string. The output string could then be accessed via the getOutput() method.
This method will return the empty string if we are in playback mode. 

We can run our program’s main method, located inside our MusicEditor class, 
to see our views. We use command line arguments to get user input.
The user calls our program with a file name and plays the given piece using
the Composite view. 

Inside our composite view, we support the current functionality:
- Selecting a note or a grid square with a mouse click
- Adding a new note at the selected square, with the ‘a’ key
- Deleting a note from the piece, with the backspace key
- Extending a note’s length, with the ‘e’ key
- Shortening a note’s length, with the ’s’ key
- Raising the pitch of a note, with the ‘+’ key
- Lowering the pitch of a note, with the ‘-‘ key
- Moving notes left and right, with the ‘m’ and ’n’ keys
- Scrolling around the composition using the arrow keys
- Play/pause, with the ‘.’ and ‘/‘ keys
- Jumping to the first and last beats of the composition, with the ‘g’ and ‘h’ keys

Our composite view also supports automatic scrolling, aka, the visible part of 
the piece will update itself once the red bar (indicating where in the piece we are)
reaches the right edge of the window. Pitch names remain static on the left side 
of the screen throughout this process. 

*********************************IMPORTANT:****************************************
* Our program runs perfectly and supports all the necessary functionality without
* error on our computers, which are both Macs. However, some aspects of our code
* have been known to produce errors when run on a PC. We have no way of testing or
* fixing this, and we did not think that it was our fault that the same Java code
* did not run the same way on different platforms. Please take this into account
* if running on a PC, and if bugs exist, try running on a Mac if possible.
***********************************************************************************


Changes Documentation (from HW05 to HW06):

Here are the changes to the model:

We made Notes immutable. This is because mutable Notes are a 
disaster for data storage and invariants.
Basically, it was hard to keep consistency in our model while the 
Notes were mutable, and there are no real benefits that mutable 
Notes gave us. We ran into problems when we mutated a Note in our
model where it didn’t reconfigure all of our settings for adding
and deleting Notes. For example, previously, when we changed the 
start time of Notes, we would then have to manually change which
timestamp the Note was mapped to and manually reset the fields;
however, by making Notes immutable and just adding and deleting Notes
when we wanted to change them, this automatically updated our model
for us. It’s also easier to maintain the invariants on the Notes if
they cannot be changed. There are fewer places where things can go 
wrong.

We made Pitch more abstract. We added a Pitch interface and an 
implementation of that. We did this so we can represent pitches not
only as an int, which is what we exclusively used previously,
but also as a String to print. This is a better design choice because
now we have a unique identifier for each Pitch (the int value) and
we have a more user friendly version that shows the exact note the
pitch is.

We revised any method that didn’t have a void return type to return
a copy of our data, rather than the actual object reference stored within
our model. This is in line with making Notes immutable, because we don’t
want clients to be able to access any Note using our getNoteAt() and 
getNotesAtTime() methods and then mutate them. This is also relevant for 
methods that return Pitches. 

We added the field instrument for our Notes, because without it there is no way to 
represent the different instruments that Notes can be. Since there can be multiple Notes 
that play at the same time but with different instruments, we also made it possible to
add Notes with the same start time and pitch as long as the instruments were all different.
We chose to make this change because without it, we wouldn’t be able to represent the
given songs properly in the Midi view.

We added the field velocity to our Notes, representing the volume of each note, for
similar reasons that we added the field instrument. Notes are given to us with
different volumes and it’s important to be able to represent all of the data.

We added a ModelToView adapter for our Model that is used in the ConsoleView. Our 
adapter takes our model and adds additional methods that facilitate printing into 
the console, such as printRow() and printPitches(). Then, our ConsoleView only needs
a ModelToView adapter in order to piece together output row by row. We did this because
our original model, while ideal for the Midi and Visual view, cannot access the continuation
of notes without first accessing the notes that start at a given time.


Changes Documentation (from HW06 to HW07):

We added a few more methods in our model relating to note access at a given time. 
Specifically, we added a method that returns all the notes that are played or 
sustained at a given time, and we added a method that returns all notes that end
at the given time. The former facilitates interaction with our MouseHandler, and the latter
facilitates MIDI playback.

We changed the mechanism by which we played notes in our MidiViewImpl. Before, we 
sent all the notes to be played all at once at the beginning. Now, we send notes
to be played immediately as soon as the timestamp reaches their starting beat. Similarly,
we send notes to be turned off as soon as the timestamp reaches their ending beat. We also
now use a synthesizer rather than a sequencer to play our notes. 

Both our ControllerImpl and our MidiViewImpl have fields that track whether or not 
the music is currently in playback mode. In playback mode, the controller’s timer is active
and will advance the timestamp according to the piece’s tempo, as well as send those notes to
be played by the Midi synthesizer. 

We added a ViewModel to act as a decorator of models, giving them added functionality
that would be useful in creating our views.












