class Array
  
  #Aliasing to retain the original functionality
  alias original_collect collect
  alias original_collect! collect!

  def collect	
    puts "In own version of collect--"
    if block_given?
      new_array = []
      each do |value|
        new_array << yield(value)+1
      end
      new_array
    else
      puts "No block given"
      self
    end
  end

  def collect!
    puts "In own version of collect!--"
    if block_given?
      each_with_index do |value, index|
        self[index] = yield(value)+1
      end
    else
      puts "No block given"
    end
    self
  end
end
