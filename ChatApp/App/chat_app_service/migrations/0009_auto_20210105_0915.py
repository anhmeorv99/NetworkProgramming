# Generated by Django 3.1.3 on 2021-01-05 09:15

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('chat_app_service', '0008_chatprivate'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='room',
            name='member',
        ),
        migrations.DeleteModel(
            name='Message',
        ),
        migrations.DeleteModel(
            name='Room',
        ),
    ]
