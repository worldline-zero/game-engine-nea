#include "../inc/timed_job.hpp"

namespace event {

  timed_job::timed_job() {}

  timed_job::timed_job(std::function<void(unsigned int, unsigned int)> job_param, const int N) :
    start_time(std::chrono::high_resolution_clock::now()),
    //job_duration(std::chrono::duration<unsigned int, std::milli>(N)),
    job_duration_ticks(N),
    job(job_param) {
    
    if (N < 0) {
      this->run_forever = true;
      this->job_duration = std::chrono::duration<unsigned int, std::milli>(0);
    } else {
      this->run_forever = false;
      this->job_duration = std::chrono::duration<unsigned int, std::milli>((unsigned int)N);
    }
  }

  void timed_job::add_to(std::map<std::string, timed_job> &jobs, std::string name) {
    this->start_time = std::chrono::high_resolution_clock::now();
    jobs.insert(std::make_pair(name, *this));
  }
    

  bool timed_job::check_expired() {
    if (!run_forever) {
      auto current_time = std::chrono::high_resolution_clock::now();
      unsigned int total_time_elapsed = std::chrono::duration_cast<std::chrono::duration<unsigned int, std::milli>>(current_time - this->start_time).count();
      //std::cout << total_time_elapsed << std::endl;
      if (total_time_elapsed >= this->job_duration.count()) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  void timed_job::run() {
    this->job(
        std::chrono::duration_cast<std::chrono::duration<unsigned int, std::milli>>(std::chrono::high_resolution_clock::now() - this->start_time).count(),
        this->job_duration_ticks
        );
  }

  void timed_job::restart() {
    this->start_time = std::chrono::high_resolution_clock::now();
  }

  
  void address_active_jobs(std::map<std::string, timed_job> &all_active_jobs) {
    for (auto i = all_active_jobs.begin(); i!=all_active_jobs.end(); i++) {
      if (i->second.check_expired() == false) {
        i->second.run();
      } else {
        i->second.run();
        i = all_active_jobs.erase(i);
      }
    }
  }

}
