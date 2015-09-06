class ShopperController < ApplicationController
  include CurrentCart

  before_action :set_cart
  skip_before_action :authorize 
  
  def index
    @products = Product.search(params[:search]).order(:name)
  end
  
end
