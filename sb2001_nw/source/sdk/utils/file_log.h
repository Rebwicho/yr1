#ifndef FILE_LOG_H
#define FILE_LOG_H

/* logging into file:
 * global-log-folder/
 *  +-----logs-owner-name( example: offsets )-folder
 *  |       |   ( example: we logged something in /modules/noswings_dll/error> this is error
 *  |       |              we logged something in /modules/noswings_dll/warning> this is warning
 *  |       |   the idea here is to set folder where we log to "logger-root" and then type of log
 *  |       |   with type-of-log@where> text
 *  |       +---modules
 *  |       |    +---errors.log   > error@noswings_dll> this is error
 *  |       |    +---warnings.log > warning@noswings_dll> this is warning
 *  |       |    |
 *  |       |    +---full.log     > error@noswings_dll> this is error
 *  |       |                       warning@noswings_dll> this is warning
 *  |       |                       ...
 *  |       +---...
 *  |       we decide what "where" is by:
 *  |        - full path( /modules/noswings_dll/error> ) - 1 ( so we exclude logs-owner ) that gives us /noswings_dll/error>
 *  |        - then we take last "/error" from it and what is left == "where"
 *  |        - so even when there is /modules/x/y/z/c/warning>
 *  |        - we get: warning@x/y/z/c>
 *  |
 *  +-----...
 *
 */

#endif // FILE_LOG_H
