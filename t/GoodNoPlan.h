// Copyright Theo Willows 2013-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

class GoodNoPlan {

  public:

    GoodNoPlan();

    ~GoodNoPlan();

    void public_function(const char* x);

  protected:

    void protected_function();

  private:

    void private_function();

};
