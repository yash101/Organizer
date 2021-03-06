;(function()
{
  $(document).ready(function()
  {
    $("body > #topnav > div#menu-toggle").click(function()
    {
      Drawer.toggle();
    });
  });
})();

var Drawer =
{
  //0=closed;1=open
  opened:false,
  open:function()
  {
    $("body > #sidebar").animate(
      {left: "0px"},
      {duration: 200, queue: false}
    );

    $("body > #content").animate(
      {left: "256px", right: "-256px"},
      {duration: 200, queue: false}
    );

    this.opened = true;
  },
  close:function()
  {
    $("body > #sidebar").animate(
    {
      left: "-256px"
    },
    {
      duration: 200,
      queue: false
    });

    $("body > #content").animate(
    {
      left: "0px",
      right: "0px"
    },
    {
      duration: 200,
      queue: false
    });

    this.opened = false;
  },
  toggle:function()
  {
    if(this.opened) this.close();
    else this.open();
  }
};
