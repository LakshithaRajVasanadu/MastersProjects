class StudentArray
  def initialize
    @students = [{:firstname => "John", :lastname => "LastnameJohn",  :phonenumber => 123456789}, 
	  	 {:firstname => "Ken", :lastname => "Lastnameken",  :phonenumber => 456734244},
		 {:firstname => "Bob", :lastname => "LastnameJohn",  :phonenumber => 456734245},
	      	 {:firstname => "Marisa", :lastname => "lastnamemarisa",  :phonenumber => 443234567},
		 {:firstname => "Ken", :lastname => "Kenlastname",  :phonenumber => 456734244},
		 {:firstname => nil, :lastname => "Kenlastname",  :phonenumber => 456734245}]
  end

  # Method to query students array. Displays all records if options are not specified
  def get_students(options={})
    firstname = options[:firstname] 
    lastname = options[:lastname]
    phonenumber = options[:phonenumber]
    results = @students
    if !firstname.nil? && !firstname.empty?
      results = results.select {|student| student[:firstname] && student[:firstname].casecmp(firstname) == 0} 
    end
    if !lastname.nil? && !lastname.empty?
      results = results.select {|student| student[:lastname] && student[:lastname].casecmp(lastname) == 0}
    end
    if !phonenumber.nil? && phonenumber != 0
      results = results.select {|student| student[:phonenumber] && student[:phonenumber] == phonenumber}
    end
    results
  end

  # Method to print students array 
  def print_students_array(students)
    if !students.empty?
      puts "Match found:"
      students.each_with_index { |student, index|
        print "#{index+1}) "
	print "First Name: #{student[:firstname]}, "
	print "Last Name: #{student[:lastname]}, "
	puts "Phone:#{student[:phonenumber]}"
      }
    else
      puts "No match found"
    end
  end

  def get_user_input
    puts "----------MENU----------"
    puts "A. Run Default Queries"
    puts "B. Run Custom Query"
    puts "------------------------"
    print "Please enter your choice:"
    choice = gets.chomp

    if choice == "A"
      run_default_queries
    elsif choice == "B"
      run_custom_query	
    end
  end

  def run_default_queries
    puts "Query 1: Running query to display all the records"
    results = get_students
    print_students_array results

    puts "\nQuery 2: Running query with First Name = Ken"	
    results = get_students({:firstname => "Ken"})
    print_students_array results

    puts "\nQuery 3: Running query with Last Name = LastnameJohn"
    results = get_students({:lastname => "LastnameJohn"})
    print_students_array results

    puts "\nQuery 4: Running query with Phone = 456734244"
    results = get_students({:phonenumber => 456734244})
    print_students_array results

    puts "\nQuery 5: Running query with First Name = bob and Last Name = LastnameJohn"
    results = get_students({:firstname => "bob", :lastname => "LastnameJohn"})
    print_students_array results

    puts "\nQuery 6: Running query with First Name = Ken and Last Name = LastnameJohn"
    results = get_students({:firstname => "Ken", :lastname => "LastnameJohn"})
    print_students_array results
  end

  def run_custom_query
    print "Enter First Name: "
    firstname = gets.chomp

    print "Enter Last Name: "
    lastname = gets.chomp

    print "Enter phone: "
    phonenumber = gets.chomp.to_i

    results = get_students({:firstname => firstname, :lastname => lastname, :phonenumber => phonenumber})
    print_students_array results
  end
end

StudentArray.new.get_user_input
