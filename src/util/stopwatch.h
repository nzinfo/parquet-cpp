// Copyright 2012 Cloudera Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PARQUET_UTIL_STOPWATCH_H
#define PARQUET_UTIL_STOPWATCH_H

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifndef CLOCK_MONOTONIC 
#define CLOCK_MONOTONIC CLOCK_REALTIME          /* OS/X is silly */ 
#endif 

namespace parquet_cpp {

class StopWatch {
 public:
  StopWatch() {
  }

  void Start() {
    //clock_gettime(CLOCK_MONOTONIC, &start_);
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  start_.tv_sec = mts.tv_sec;
  start_.tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, &start_);
#endif
  }

  // Returns time in nanoseconds.
  uint64_t Stop() {
    timespec end;
    //clock_gettime(CLOCK_MONOTONIC, &end);
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    end.tv_sec = mts.tv_sec;
    end.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &end);
#endif
    return (end.tv_sec - start_.tv_sec) * 1000L * 1000L * 1000L +
           (end.tv_nsec - start_.tv_nsec);
  }

 private:
  timespec start_;
};

}

#endif

