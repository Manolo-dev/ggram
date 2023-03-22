bool _loop(bool (*f)()) { if(f()) return 1; while(1) if(f()) break; return 0; }
bool __(string val) { if(it == it_e) return 1; if(it->value() != val) return 1; it++; return 0; }