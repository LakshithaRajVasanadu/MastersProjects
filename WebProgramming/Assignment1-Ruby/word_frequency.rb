class WordFrequency
  def initialize
    @sentence = "Here is a list of few famous tongue twisters:
  		Peter Piper picked a peck of pickled peppers.				
		A peck of pickled peppers Peter Piper picked.
		If Peter Piper picked a peck of pickled peppers,
		Where's the peck of pickled peppers Peter Piper picked?

		I saw Susie sitting in a shoe shine shop.
		Where she sits she shines, and where she shines she sits.

		How can a clam cram in a clean cream can?

		Denise sees the fleece,
		Denise sees the fleas.
		At least Denise could sneeze
		and feed and freeze the fleas"

    calculate_frequency
  end

  def calculate_frequency
    @freq_hash = {}
    @freq_hash = each_word.each_with_object(Hash.new(0)) { |word,counts| counts[word] += 1 }
  end

  def get_frequency(word)
    @freq_hash[word].nil? ? 0 : @freq_hash[word] 
  end

  def print_word_frequencies
    @freq_hash.sort.each {|word, frequency|
      puts "Word: #{word} - Frequency: #{frequency}"
    }
  end

  def get_user_input
    puts "----------MENU----------"
    puts "A. Find frequencies of all words"
    puts "B. Find frequency of particular word"
    puts "------------------------"
    print "Please enter your choice:"
    choice = gets.chomp

    if choice == "A"
      print_word_frequencies
    elsif choice == "B"
      print "Enter word: "
      puts "Frequency: #{get_frequency(gets.chomp.downcase)}"	
    end
  end

  private
    # Method to preprocess and split the sentence
    def each_word
      @sentence.downcase.scan(/\w+/)
    end
end

WordFrequency.new.get_user_input
