
/**
 * @file Task.cpp
 * @brief Implementation of class Task
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

#include <pspkernel.h>

#include "Task.h"

#include <sstream>

using namespace std;

static const char* today()
{
    time_t now;
    struct tm* tmnow;
    static char sznow[11];

    sceKernelLibcTime(&now);
    tmnow = localtime(&now);

    strftime(sznow, 11, "%Y-%m-%d", tmnow);

    return sznow;
}

Task::Task(const string& ID,
           const string& subject,
           const string& startDate,
           const string& dueDate,
           const string& completionDate,
           bool dirty)
    : m_ID(ID),
      m_Subject(subject), // TMP
      m_StartDate(startDate),
      m_DueDate(dueDate),
      m_CompletionDate(completionDate),
      m_bDirty(dirty)
{
}

Task::~Task()
{
    for (list<Task*>::iterator k = m_Children.begin();
         k != m_Children.end();
         ++k)
    {
       delete *k;
    }
}

void Task::addChild(Task *t)
{
    m_Children.push_back(t);
}

const string& Task::subject()
{
    return m_Subject;
}

list<Task*>& Task::children()
{
    return m_Children;
}

bool Task::completed()
{
    return m_CompletionDate != "None";
}

bool Task::overdue()
{
    return m_DueDate < today() && !completed();
}

bool Task::inactive()
{
    return m_StartDate > today() && !completed();
}

bool Task::active()
{
    return !inactive() && !completed();
}

bool Task::dueToday()
{
    return m_DueDate == today() && !completed();
}

void Task::markCompleted()
{
    if (!completed())
    {
       m_CompletionDate = today();
       m_bDirty = true;

       for (list<Task*>::iterator k = m_Children.begin();
            k != m_Children.end();
            ++k)
       {
          (*k)->markCompleted();
       }
    }
}

void Task::write(FILE *fp, unsigned int level)
{
    for (unsigned int i = 0; i < level; ++i)
       fprintf(fp, " ");

    fprintf(fp, "%d %s %s %s %s %s\n",
            m_bDirty ? 1 : 0,
            m_ID.c_str(),
            m_StartDate.c_str(),
            m_DueDate.c_str(),
            m_CompletionDate.c_str(),
            m_Subject.c_str());

    for (list<Task*>::iterator k = m_Children.begin();
         k != m_Children.end();
         ++k)
    {
       (*k)->write(fp, level + 1);
    }
}

static const char* _rcsid_Task __attribute__((unused)) = "$Id$";
