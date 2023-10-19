namespace level {

  template <typename T>
  void flags_parser::operator()(Tokenizer::iterator &i, Level *l, T &object) {
    while (*(++i) != "]") {
      if (*i == "start") {
        this->functions.insert(std::make_pair("start", 0u));
      } else if (*i == "end") {
        this->functions.insert(std::make_pair("end", 0u));
      } else if (*i == "moving") {
        i++;
        glm::vec3 new_pos = parse_vec3(i);
        float d = boost::lexical_cast<float>(*(++i));
        this->functions.insert(std::make_pair("moving", glm::vec4(new_pos, d)));
      }
    }
  }


}
