class Enclose
  attr_reader :enclosed_words

  #Constructor for the class
  def initialize(start_ch, end_ch, *words)
    @enclosed_words = Array.new
    begin
      words.each{|word| @enclosed_words << start_ch + word + end_ch }
    rescue Exception=>e
      puts "Exception occurred: #{e.message}"
    end
  end

  #Method to reset the values
  def set(start_ch, end_ch, *words)
    @enclosed_words.clear if start_ch && end_ch
    begin
      words.each{|word| @enclosed_words << start_ch + word + end_ch }
        rescue Exception=>e
      puts "Exception occurred: #{e.message}"
    end
  end
  
end
