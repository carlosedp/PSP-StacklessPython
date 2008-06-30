
/**
 * @file TaskFile.cpp
 * @brief Implementation of class TaskFile
 */

/**********************************************************************

  Created: 17 May 2008

    Copyright (C) 2008 Frank Buss, Jérôme Laheurte

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The names of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************/
// $Id$

#include "TaskFile.h"
#include "Task.h"

#include <stack>

#include <ctype.h>

using namespace std;

TaskFile::TaskFile(const string& filename)
    : m_Filename(filename)
{
}

TaskFile::~TaskFile()
{
}

vector<Task*> TaskFile::read()
{
    stack<Task*> parents;
    Task *pTask = 0;
    char szLine[4096];
    vector<Task*> tasks;
    unsigned int indentLevel = 0;

    FILE *fp = fopen(m_Filename.c_str(), "r");
    if (!fp)
       return tasks;

    while (fgets(szLine, 4096, fp))
    {
       unsigned int newLevel = 0;

       szLine[strlen(szLine) - 1] = 0;

       while ((newLevel < strlen(szLine)) && (isspace(szLine[newLevel])))
          ++newLevel;

       if (newLevel < indentLevel)
       {
          for (unsigned int i = 0; i < indentLevel - newLevel; ++i)
          {
             parents.pop();
          }
       }
       else if (newLevel > indentLevel)
       {
          parents.push(pTask);
       }

       pTask = _parseLine(szLine + newLevel);

       if (parents.empty())
       {
          tasks.push_back(pTask);
       }
       else
       {
          parents.top()->addChild(pTask);
       }

       indentLevel = newLevel;
    }

    fclose(fp);

    return tasks;
}

Task* TaskFile::_parseLine(char *szLine)
{
    // <dirty flag> <startDate> <dueDate> <completionDate> <subject>

    bool dirty;
    string ID, startDate, dueDate, completionDate;
    char *next;

    dirty = atoi(szLine);
    while (!isspace(*szLine)) ++szLine;
    while (isspace(*szLine)) ++szLine;

    next = szLine;
    while (!isspace(*next)) ++next;
    *next++ = 0;
    while (isspace(*next)) ++next;
    ID = szLine;
    szLine = next;

    next = szLine;
    while (!isspace(*next)) ++next;
    *next++ = 0;
    while (isspace(*next)) ++next;
    startDate = szLine;
    szLine = next;

    next = szLine;
    while (!isspace(*next)) ++next;
    *next++ = 0;
    while (isspace(*next)) ++next;
    dueDate = szLine;
    szLine = next;

    next = szLine;
    while (!isspace(*next)) ++next;
    *next++ = 0;
    while (isspace(*next)) ++next;
    completionDate = szLine;
    szLine = next;

    return new Task(ID, szLine, startDate, dueDate, completionDate, dirty);
}

static const char* _rcsid_TaskFile __attribute__((unused)) = "$Id$";
