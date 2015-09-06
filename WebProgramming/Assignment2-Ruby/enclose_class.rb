class EncloseClass

  @enclosed_words = Array.new

  #Class constructor
  def initialize(start_ch, end_ch, *words)
    EncloseClass.initialize_class(start_ch, end_ch, *words)
  end

  def self.initialize_class(start_ch, end_ch, *words)
    @enclosed_words.clear
    begin
      words.each{|word| @enclosed_words << start_ch + word + end_ch }
    rescue Exception=>e
      puts "Exception occurred: #{e.message}"
    end
  end

  #Method to reset the values
  def self.set(start_ch, end_ch, *words)
    @enclosed_words.clear if start_ch && end_ch
    begin
      words.each{|word| @enclosed_words << start_ch + word + end_ch }
    rescue Exception=>e
      puts "Exception occurred: #{e.message}"
    end
  end

  #Method to reverse the elements
  def self.reverse_strings
    @enclosed_words.reverse
  end

  def self.enclosed_words
    @enclosed_words
  end
end
