#include <stdio.h>
#include <string.h>

#include <windows.h>

// The important things to note and know:
//
//  * Only works with ASCII paths and filenames.
//  * Quotes all arguments regardless of content.
//  * SW_HIDE in STARTUPINFO is needed for GUI applications
//  * DETACHED_PROCESS in CreateProcessA is needed for command line applications
//

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		printf("daemonize.exe COMMAND [ARGUMENTS]");
	}
	
	char* command = argv[1];
	char* arguments = NULL;
	
	if (argc > 2) {
		// We only need to concatenate arguments if there
		// actually are arguments.
		int idx;

		// Calculate the length of all arguments
		size_t argLength = 0;
		for (idx = 1; idx < argc; idx++) {
			argLength += strlen(argv[idx] + 2 + 1); // One for the space, two for the quotes.
		}
		argLength--; // Strip the last space.

		// Reserve some memory and NULL the new string.
		//The + 1 is the space for the last null character.
		arguments = (char*)malloc(sizeof(char) * (argLength + 1));
		arguments[0] = 0;
		
		// Now concatenate the arguments.
		for (idx = 1; idx < argc; idx++) {
            strcat(arguments, "\"");
			strcat(arguments, argv[idx]);
			strcat(arguments, "\"");
			if (idx < argc - 1) {
				strcat(arguments, " ");
			}
		}
	}

	STARTUPINFO startInfo;
	PROCESS_INFORMATION process;
	
	ZeroMemory(&startInfo, sizeof(startInfo));
	ZeroMemory(&process, sizeof(process));
	
	startInfo.cb = sizeof(startInfo);
	
	// Tell the system to use/honor the ShowWindow flags.
	startInfo.dwFlags = STARTF_USESHOWWINDOW;
	// Tell the system that the main window of the process should be hidden.
	startInfo.wShowWindow = SW_HIDE;
	
	if (!CreateProcessA(
			command,			// application name
			arguments,			// command line arguments
			NULL,				// process attributes
			NULL,				// thread attributes
			FALSE,				// inherit (file) handles
			// Detach the process from the current console.
			DETACHED_PROCESS,   // creation flags
			NULL,				// environment
			NULL,				// current directory
			&startInfo,			// startup info
			&process)			// process information
		) {

		printf("Creation of the process failed, trying to fetch error message...\n");

		long lastError = GetLastError();
		char* message = NULL;
		
		FormatMessageA(
		    FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_ARGUMENT_ARRAY |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, // Flags
		    NULL,               // source
			lastError,          // message identifier
			0,                  // language id
			(LPSTR)&message,   // message buffer
			0,                  // size of the message
			NULL                // arguments
		);
		
		printf(message);
		
		return 1;
	}
	
	return 0;
}
