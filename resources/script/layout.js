;var ContentPane =
{
  getBackgroundColor:function()
  {
    return $("body > #content").css("background-color");
  },
  setBackgroundColor:function(color)
  {
    $("body > #content").css("background-color", color);
  },
  getTextColor:function()
  {
    return $("body > #content").css("background-color");
  },
  setTextColor:function(color)
  {
    $("body > #content").css("color", color);
  },
  jQueryHandle:function()
  {
    return $("body > #content");
  }
};
