require_relative 'enclose'
require_relative 'compress'
require_relative 'enclose_class'
require_relative 'compress_class'
require_relative 'array'
require_relative 'compress_enumerable'

#Demo for Question 1
def enclose_demo
  puts "-----QUESTION 1) ENCLOSE DEMO START-----"
  puts "Creating new enclosed string------"
  enclose_obj = Enclose.new("<", ">", "how", "are", "you", "doing?")

  #Singleton method for object
  def enclose_obj.reverse_strings
    self.enclosed_words.reverse
  end

  puts "Enclosed array of strings are: "
  p enclose_obj.enclosed_words

  puts "\nSetting the strings to new value:"
  enclose_obj.set("{", "}", "i", "am", "fine")

  puts "New enclosed array of strings are: "
  p enclose_obj.enclosed_words

  puts "\nReversed array contents using object singleton method are: "
  p enclose_obj.reverse_strings

  #Creating empty array
  puts "\nCreating new enclosed string 2(empty list of words)------" 
  enclose_obj2 = Enclose.new("<", ">")

  puts "\nEnclosed array of strings are: "
  p enclose_obj2.enclosed_words

  #Creating class without delimiters - Exception handling 
  puts "\nCreating new enclosed string 3 to demo exception handling------" 
  enclose_obj3 = Enclose.new(nil, nil)

  puts "\nSetting the strings to new value:"
  enclose_obj.set("<", ">", "i", "am", "fine", "now")
  
  puts "Enclosed array of strings are: "
  p enclose_obj3.enclosed_words

  puts "\nSetting the strings to new value:"
  enclose_obj.set(nil, nil, "i", "am", "fine", "now")
  
  puts "Enclosed array of strings are: "
  p enclose_obj3.enclosed_words

  puts "-----ENCLOSE DEMO END-----"
end

#Demo for Question 2
def compress_demo
  puts "\n-----QUESTION 2) COMPRESS DEMO START-----"
  puts "Creating new compressed string------"
  cobj = Compress.new("i love you but do you love me")
  puts "Object state variables are: "
  p cobj.comp_str
  p cobj.index

  puts "\nCreating new compressed string with other whitespace characters------"
  puts "Creating new compressed string------"
  cobj2 = Compress.new("i love you	;but do you love me")
  puts "Object state variables are: "
  p cobj2.comp_str
  p cobj2.index

  puts "\nCreating new compressed string empty string------"
  puts "Creating new compressed string------"
  cobj3 = Compress.new("")
  puts "Object state variables are: "
  p cobj3.comp_str
  p cobj3.index

  puts "\nCreating new compressed string with nil string------"
  puts "Creating new compressed string------"
  cobj4 = Compress.new(nil)
  puts "Object state variables are: "
  p cobj4.comp_str
  p cobj4.index
  puts "-----COMPRESS DEMO END-----"
end

#Demo for Question 3 - Enclose
def enclose_class_demo
  puts "\n-----QUESTION 3a) ENCLOSE CLASS DEMO START-----"
  puts "Creating new enclosed string------"
  enclose_obj = EncloseClass.new("<", ">", "how", "are", "you", "doing?")

  puts "Enclosed array of strings are: "
  p EncloseClass.enclosed_words

  puts "\nSetting the strings to new value:"
  EncloseClass.set("{", "}", "i", "am", "fine")

  puts "New enclosed array of strings are: "
  p EncloseClass.enclosed_words

  puts "\nReversed array contents are: "
  p EncloseClass.reverse_strings

  #Creating empty array
  puts "\nCreating new enclosed string 2(empty list of words)------" 
  enclose_obj2 = EncloseClass.new("<", ">")

  puts "\nEnclosed array of strings are: "
  p enclose_obj2.class.enclosed_words

  #Creating class without delimiters - Exception handling 
  puts "\nCreating new enclosed string 3 to demo exception handling------" 
  enclose_obj3 = EncloseClass.new(nil, nil)

  puts "\nSetting the strings to new value:"
  enclose_obj.class.set("<", ">", "i", "am", "fine", "now")
  
  puts "Enclosed array of strings are: "
  p enclose_obj3.class.enclosed_words

  puts "\nSetting the strings to new value:"
  enclose_obj.class.set(nil, nil, "i", "am", "fine", "now")
  
  puts "Enclosed array of strings are: "
  p enclose_obj3.class.enclosed_words

  puts "-----ENCLOSE CLASS DEMO END-----"
end

#Demo for Question 3 - Compress
def compress_class_demo
  puts "\n-----QUESTION 3b) COMPRESS CLASS DEMO START-----"
  puts "Creating new compressed string------"
  cobj = CompressClass.new("i love you but do you love me")
  puts "Variables are: "
  p CompressClass.comp_str
  p CompressClass.index

  puts "\nCreating new compressed string with other whitespace characters------"
  puts "Creating new compressed string------"
  cobj2 = CompressClass.new("i love you	;but do you love me")
  puts "Variables are: "
  p CompressClass.comp_str
  p CompressClass.index

  puts "\nCreating new compressed string empty string------"
  puts "Creating new compressed string------"
  cobj3 = CompressClass.new("")
  puts "Variables are: "
  p cobj3.class.comp_str
  p cobj3.class.index

  puts "\nCreating new compressed string with nil string------"
  puts "Creating new compressed string------"
  cobj4 = CompressClass.new(nil)
  puts "Variables are: "
  p cobj4.class.comp_str
  p cobj4.class.index
  puts "-----COMPRESS CLASS DEMO END-----"
end

#Demo for Question 4
def array_collect_demo
  puts "\n-----ARRAY COLLECT DEMO START-----"
  puts "Original Array collect demo"
  a = [1,2,3]
  puts "Array is:"
  p a
  puts "Using collect to get squares of elements"
  result = a.original_collect{|ele| ele*ele }
  puts "Result is:"
  p result
  puts "Original Array(Unchanged):"
  p a

  puts "\nOwn version of Array collect demo"
  a = [1,2,3]
  puts "Array is:"
  p a
  puts "Using collect to get squares of elements"
  result = a.collect{|ele| ele*ele }
  puts "Result is:"
  p result
  puts "Original Array(Unchanged):"
  p a

  puts "Without block demo"
  p [1,2,3].collect

  puts "\nOriginal Array collect! demo"
  a = [1,2,3]
  puts "Array is:"
  p a
  puts "Using collect to get squares of elements"
  result = a.original_collect!{|ele| ele*ele }
  puts "Result is:"
  p result
  puts "Original Array(Changed):"
  p a

  puts "\nOwn version of Array collect! demo"
  a = [1,2,3]
  puts "Array is:"
  p a
  puts "Using collect to get squares of elements"
  result = a.collect!{|ele| ele*ele }
  puts "Result is:"
  p result
  puts "Original Array(Changed):"
  p a

  puts "Without block demo"
  p [1,2,3].collect!

  puts "\n-----ARRAY COLLECT DEMO END-----"
end

# Demo for Question 5
def compress_with_enumerable_demo
  puts "\n-----QUESTION 5) COMPRESS ENUMERABLE DEMO START-----"
  puts "Creating new compressed string------"
  cobj = CompressEnumerable.new("i love you but do you love me")
  puts "Object state variables are: "
  p cobj.comp_str
  p cobj.index

  puts "Demo of each function"
  cobj.each{|word| puts "Word is #{word}"}

  puts "Longest word: #{cobj.max{|word1, word2| word1.length <=> word2.length }}"

  puts "Original String re-constructed: #{cobj.inject{|string, word| string = string + " " + word }}"

  puts "Reversing the elements:"
  cobj.reverse_each{|word| print "#{word} " }

  puts "\nCreating new compressed string 2------"
  cobj2 = CompressEnumerable.new("parents are the best")
  puts "Object state variables are: "
  p cobj2.comp_str
  p cobj2.index

  puts "Demo of each function"
  cobj2.each{|word| puts "Word is #{word}"}
  
  puts "Longest word: #{cobj2.max{|word1, word2| word1.length <=> word2.length }}"

  puts "Original String re-constructed: #{cobj2.inject{|string, word| string = string + " " + word }}"

  puts "Reversing the elements:"
  cobj2.reverse_each{|word| print "#{word} " }

  puts "\nCreating new compressed string 3 empty string------"
  cobj3 = CompressEnumerable.new("")
  puts "Object state variables are: "
  p cobj3.comp_str
  p cobj3.index

  puts "Demo of each function"
  cobj3.each{|word| puts "Word is #{word}"}
  
  puts "Longest word: #{cobj3.max{|word1, word2| word1.length <=> word2.length }}"

  puts "Original String re-constructed: #{cobj3.inject{|string, word| string = string + " " + word }}"

  puts "-----COMPRESS ENUMERABLE DEMO END-----"
end

#Calling methods for demo
enclose_demo  					#Question 1
compress_demo       				#Question 2
enclose_class_demo 				#Question 3a
compress_class_demo 				#Question 3b
array_collect_demo  				#Question 4
compress_with_enumerable_demo 			#Question 5
