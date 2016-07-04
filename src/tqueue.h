// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// tqueue.h - template classes for Queue and QItem

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
/*
#ifndef TQUEUE_H
#define TQUEUE_H

#include "new_baseclass.h"

// Forward declarations.
template<class qdata_t> class Queue;

// Template for Queue.
template<class qdata_t> class Queue : public class_metamod_new {
	private:
	// private copy/assign constructors:
		Queue(const Queue &src);
		void operator=(const Queue &src);
	protected:
	// structs:
		class QItem : public class_metamod_new {
			private:
			// private copy/assign constructors:
				QItem(const QItem &src);
				void operator=(const QItem &src);
			public:
				qdata_t *data;
				QItem *next;
				QItem(void) :data(NULL), next(NULL) { };
				QItem(qdata_t *dnew) :data(dnew), next(NULL) { };
		};
	// data:
		int size;
		QItem *front;
		QItem *end;
	public:
	// constructor:
		Queue(void) :size(0), front(NULL), end(NULL) {};
	// functions:
		void push(qdata_t *qadd);
		qdata_t * pop(void);
};


///// Template Queue:

// Push onto the queue (at end).
template<class qdata_t> inline void Queue<qdata_t>::push(qdata_t *qadd) {
	QItem *qnew = new QItem(qadd);
	
	if(size==0)
		front=qnew;
	else
		end->next=qnew;
	
	end=qnew;
	
	size++;
}

// Pop from queue (from front).  Wait for an item to actually be available
// on the queue (block until there's something there).
template<class qdata_t> inline qdata_t* Queue<qdata_t>::pop(void) {
	QItem *qtmp;
	qdata_t *ret;
	
	if(size==0)
		return(NULL);
	
	qtmp=front;
	
	ret=front->data;
	front=front->next;
	
	delete qtmp;
	
	size--;
	
	return(ret);
}

#endif*/ /* TQUEUE_H */
