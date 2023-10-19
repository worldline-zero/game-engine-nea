#ifndef TIMED_JOB_H
#define TIMED_JOB_H

#include <chrono>
#include <vector>
#include <functional>
#include <iostream>

#define JOB_SIGNATURE void(unsigned int, unsigned int)

namespace event {

  class timed_job {

    public:

      std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
     
      std::chrono::duration<unsigned int, std::milli> job_duration;

      int job_duration_ticks;

      bool run_forever;

      std::function<JOB_SIGNATURE> job;

      timed_job();

      timed_job(std::function<JOB_SIGNATURE> job_param, const int N);

      void add_to(std::vector<timed_job> &jobs);

      bool check_expired();

      void run();

      void restart();

      constexpr timed_job &operator=(const timed_job &other) {
        this->start_time = other.start_time;
        this->job_duration = other.job_duration;
        this->job_duration_ticks = other.job_duration_ticks;
        this->run_forever = other.run_forever;
        this->job = other.job;
        return *this;
      }

  };

  void address_active_jobs(std::vector<timed_job> &all_active_jobs);

}

#endif
