class Compress
  
  # Class constructor
  def initialize(str)
    @comp_str, @index = [], []
    unless str.nil?
      words = str.scan(/\w+/)
      @comp_str = words.uniq
      @index = words.map{ |word| @comp_str.index(word)}
    end
  end

  # Getter for unique string words
  def comp_str
    @comp_str
  end

  # Getter for indices array
  def index
    @index
  end
end
