// Copyright Theo Willows 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

class Good {

  public:

    Good();

    ~Good();

    void public_function(const char* x);

  protected:

    void protected_function();

  private:

    void private_function();

};
