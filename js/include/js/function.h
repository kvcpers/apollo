#pragma once

#include "js/value.h"
#include "js/context.h"
#include "js/ast.h"
#include <vector>
#include <string>

namespace js {

class Function : public Value {
public:
    Function();
    ~Function();

    void set_name(const std::string& name);
    std::string get_name() const;
    void set_body(std::shared_ptr<ast::BlockStatement> body);
    std::shared_ptr<ast::BlockStatement> get_body() const;
    void add_parameter(const std::string& param);
    std::vector<std::string> get_parameters() const;
    std::shared_ptr<Value> call(std::shared_ptr<Context> context, 
                               const std::vector<std::shared_ptr<Value>>& args);

private:
    std::string name_;
    std::shared_ptr<ast::BlockStatement> body_;
    std::vector<std::string> parameters;
};

} // namespace js
