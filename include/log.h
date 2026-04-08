class utils {
    public:
        utils();
        void debug(const char* name, auto& value);
        template<typename... params>
        void logger(char* first, params... args) {
            
        }
};
