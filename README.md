# CallLogger
Super quick and super ugly call logger to make work slightly easier.

Just copy call notes in, and build a call log fast! 
Make sure whatever notes you are copying and pasting put the Time the call was made first, and the Phone Number second. 
One call at a time. Also, it helps to have a country code in your notes. 

V0.0.0 > Demo released. Parses and adds raw call notes to a CallLog.csv file. Automatically adds todays date. Has an "Open .CSV" button to inspect the .csv in notepad. 

V0.0.1 > Added Apostraphes to numbers so Excel doesn't try to turn phone numbers into math functions, and IDs into scientific notation. 
Removed the "Open CSV" Button (as system restrictions at work break it) and replaced with "Remove last line" button. 

V0.0.2 > Added a "Show Last Line" box since I keep forgetting where I am. 

V0.0.3 > Added (Working) "Open Current Log" Button. Added (Not Working) "Clear Current Log" and "Stamp Current Log" buttons, split About and Help into two buttons, added Settings menu to save weekdays.

Planned Features (Warning: Doing this for fun in my free time. Updates will be slow, if at all)

1.) Date box showing the current date, with override options to add calls from previous days.

2.) Create Diffent .CSVs for each week, with date in the name. Include override. Possibly including settings to set "workdays."

3.) Improved layout and "graphics." Improved code (right now it is spaghetti all shoved in main.cpp)

4.) Possible AI "Learn your note-taking style" feature which learns how to parse based off the user. 
Might be too ambitious, given the average user's hardware.

