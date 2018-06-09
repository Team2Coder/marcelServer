/*
 * Copyright (c) 2012 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "Mythread.h"

using namespace std;
/* Returns true if the thread was successfully started,
 * false if there was an error starting the thread */

bool MyThread::StartInternalThread() {
  int r = pthread_create(&_thread, NULL, InternalThreadEntryFunc, this);
  if (r != 0) {
    cout <<"could not start internal thread\n";
  }
  return r;
}

/** Will not return until the internal thread has exited. */
void MyThread::WaitForInternalThreadToExit() {
  (void) pthread_join(_thread, NULL);
}

void* MyThread::InternalThreadEntryFunc(void* This) {
  reinterpret_cast<MyThread*>(This)->InternalThreadEntry();
  return NULL;
}
