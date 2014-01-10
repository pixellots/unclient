#include "maccommander.h"

#include <sys/stat.h>
#include <unistd.h>
#include <Security/Authorization.h>
#include <Security/AuthorizationTags.h>

using namespace UpdateNode;

bool MacCommander::isProcessElevated()
{
    return false;
}

uint MacCommander::runProcessElevated(const QString &path,
                               const QStringList &parameters,
                               const QString &description,
                               const QString &icon)
{
    QString newDescription = description + QObject::tr(" wants to make changes.");
    int retVal = 1;

    OSStatus status;
    AuthorizationRef authRef;

    AuthorizationItem right = { "com.updatenode.unclient", 0, NULL, 0 };
    AuthorizationRights rightSet = { 1, &right };

    AuthorizationEnvironment myAuthorizationEnvironment;
    AuthorizationItem kAuthEnv[2];
    myAuthorizationEnvironment.items = kAuthEnv;

    char **commandLine;
    commandLine = new char*[parameters.size() + 1];
    for (int i = 0; i < parameters.size(); i++) {
        commandLine[i] = new char[strlen(parameters.at(i).toStdString().c_str())+1];
        memcpy(commandLine[i], parameters.at(i).toStdString().c_str(), strlen(parameters.at(i).toStdString().c_str())+1);
    }
    commandLine[parameters.size()] = ((char)NULL);

    char *prompt = NULL;
    size_t promptLen = newDescription.length();
    char *newPrompt = (char*)malloc(sizeof(char) * (promptLen + 2));
    strcpy(newPrompt, newDescription.toStdString().c_str());
    newPrompt[promptLen] = '\0';
    prompt = newPrompt;

    myAuthorizationEnvironment.count = 0;

    if(!newDescription.isEmpty())
    {
        kAuthEnv[0].name = kAuthorizationEnvironmentPrompt;
        kAuthEnv[0].valueLength = strlen(prompt);
        kAuthEnv[0].value = prompt;
        kAuthEnv[0].flags = 0;

        myAuthorizationEnvironment.count += 1;
    }

    if(!icon.isEmpty())
    {
        kAuthEnv[myAuthorizationEnvironment.count].name = kAuthorizationEnvironmentIcon;
        kAuthEnv[myAuthorizationEnvironment.count].valueLength = strlen(icon.toStdString().c_str());
        kAuthEnv[myAuthorizationEnvironment.count].value = (void*)icon.toStdString().c_str();
        kAuthEnv[myAuthorizationEnvironment.count].flags = 0;
        myAuthorizationEnvironment.count += 1;
    }

    if (AuthorizationCreate(NULL, &myAuthorizationEnvironment/*kAuthorizationEmptyEnvironment*/, kAuthorizationFlagDefaults, &authRef) != errAuthorizationSuccess)
    {
        //NSLog(@"Could not create authorization reference object.");
        status = errAuthorizationBadAddress;
    }
    else
    {
        status = AuthorizationCopyRights(authRef, &rightSet, &myAuthorizationEnvironment/*kAuthorizationEmptyEnvironment*/,
                                         kAuthorizationFlagDefaults | kAuthorizationFlagPreAuthorize
                                         | kAuthorizationFlagInteractionAllowed | kAuthorizationFlagExtendRights,
                                         NULL);
    }

    if (status == errAuthorizationSuccess)
    {
        FILE *ioPipe;
        char buffer[1024];
        int bytesRead;

        status = AuthorizationExecuteWithPrivileges(authRef, path.toStdString().c_str(), 0, commandLine, &ioPipe);

        for (;;)
        {
            bytesRead = fread(buffer, sizeof(char), 1024, ioPipe);
            if (bytesRead < 1) break;
            write(STDOUT_FILENO, buffer, bytesRead * sizeof(char));
        }

        pid_t pid;
        int pidStatus;
        do {
            pid = wait(&pidStatus);
        } while (pid != -1);

        if (status == errAuthorizationSuccess)
        {
            retVal = 0;
        }
    }
    else
    {
        AuthorizationFree(authRef, kAuthorizationFlagDestroyRights);
        authRef = NULL;
        if (status != errAuthorizationCanceled)
        {
            // pre-auth failed
            //NSLog(@"Pre-auth failed");
        }
    }

    // delete output
    int i = 0;
    while (commandLine[i]) {
        delete commandLine[i];
        i++;
    }
    delete commandLine;

    // delete prompt
    if (prompt) {
            free(prompt);
        }

    return retVal;
}
