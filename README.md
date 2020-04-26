# CallLogger
***V0.1.0***
Super quick and super ugly call logger to make work slightly easier.

Just copy call notes in, and build a call log fast! 
Make sure whatever notes you are copying and pasting put the Time the call was made first, and the Phone Number second. 
One call at a time. Also, it helps to have a country code in your notes. 

File Menu allows you to open or clear the current log, or "Stamp" it and add details on Total number of Cases and Calls.

***Release Notes***

V0.0.0 > Demo released. Parses and adds raw call notes to a CallLog.csv file. Automatically adds todays date. Has an "Open .CSV" button to inspect the .csv in notepad. 

V0.0.1 > Added Apostraphes to numbers so Excel doesn't try to turn phone numbers into math functions, and IDs into scientific notation. 
Removed the "Open CSV" Button (as system restrictions at work break it) and replaced with "Remove last line" button. 

V0.0.2 > Added a "Show Last Line" box since I keep forgetting where I am. 

V0.0.3 > Added (Working) "Open Current Log" Button. Added (Not Working) "Clear Current Log" and "Stamp Current Log" buttons, split About and Help into two buttons, added Settings menu to eventually save weekdays.

V0.0.4-6 > Added Icons and Resources, "Clear Current Log" and "Stamp Current Log" buttons working. 

V0.0.7 > Completed settings menu, now able to save and update settings. 

V0.0.8 > Auto-splits call logs each week, altering the CSV to match the normal Workdays in the settings.

V0.0.9 > Date box shos current date, with Shift/Reset buttons to alter adding date. Will also change Csvs as needed, if auto-splitting enabled.

V0.1.0 > Alpha! Logo added, slight tweak to the layout and visuals. Ready for use!

***Planned Features (Warning: Doing this for fun in my free time. Updates will be slow, if at all)***

1.) Improved code (right now it is spaghetti all shoved in main.cpp)

2.) Possible AI "Learn your note-taking style" feature which learns how to parse based off the user. 
Might be too ambitious, given the average user's hardware.

3.) Miscellaneous Improvements. Maybe a sorting feature to sort csvs by date, although Excel can already do that. Suggestions?

