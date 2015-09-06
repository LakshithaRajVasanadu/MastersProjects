class CompressClass
  @comp_str, @index = [], []

  #Class constructor
  def initialize(str)
    CompressClass.comp_str.clear
    CompressClass.index.clear
    CompressClass.initialize_class(str)
  end

  def self.initialize_class(str)
    unless str.nil?
      words = str.scan(/\w+/)
      @comp_str = words.uniq
      @index = words.map{ |word| @comp_str.index(word)}
    end
  end

  def self.comp_str
    @comp_str
  end

  def self.index
    @index
  end

end
