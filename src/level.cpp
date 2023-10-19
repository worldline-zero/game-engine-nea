#include "../inc/level.hpp"

namespace level {

  Level::Level(const std::string level_path) : running(false) {
    std::ifstream file(level_path);
    std::string src((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    boost::char_separator<char> sep("\n ");
    Tokenizer tokens(src, sep);
    std::string recon = "";

    flags_parser fp;

    sdf::AABB bounds;

    for (Tokenizer::iterator i = tokens.begin(); i!=tokens.end(); i++) {
      /*
      file >> s;
      if (s == "vec3") {
        std::cout << parse_vec3(file) << std::endl;
      }
      if (s == "cuboid") {
        std::unique_ptr<sdf::Cuboid> c = parse_cuboid(file).value();
      } else if (s == "sphere") {
        std::cout << "sphere" << std::endl;
      } else {
        std::cout << s << std::endl;
      }

      if (file.peek() == EOF) {
        std::cout << "end" << std::endl;
      }
      */


      if (*i == "cuboid") {
        sdf::Cuboid c = *parse_cuboid(i);
        fp(i, this, c);
        /*
        for (const auto &[fn_name, fn] : fp.functions) {
          if (fn_name == "start") {
            auto &local_jobs_ref = this->jobs;
            auto &local_running = this->running;
            std::cout << "start function" << std::endl;
            c.collision_behaviour = [&c, &local_jobs_ref, &local_running](physics::collision_info cb) {
              std::cout << "collided" << std::endl;
              if (c.collided == false) {
                std::cout << "access c" << std::endl;
                c.opt_timed_job = event::timed_job(
                  [&local_jobs_ref, &local_running](unsigned int ct, unsigned int tt) {
                    std::cout << ct << std::endl;
                    if (ct > 5000) {
                      local_running = false;
                    }
                  }, 5000);
                c.opt_timed_job->add_to(local_jobs_ref);
                std::cout << "access this" << std::endl;
                c.collided = true;
              }
            };
          }
        }
        */
        //c->collision_behaviour(test_info);
        //c->collided = false;
        bounds.add_object(c);
      } else if (*i == "sphere") {
        sdf::Sphere c = *parse_sphere(i);
        fp(i, this, c);
        bounds.add_object(c);
      } else {
        //std::cout << *i << std::endl;
      }


    }

    this->scene.add_volume(bounds);

  }

}
